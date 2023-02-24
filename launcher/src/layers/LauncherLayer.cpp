#include "layers/LauncherLayer.h"
#include "project/ProjectGenerator.h"

#include <imgui.h>
#include <imgui_stdlib.h>

namespace Calyx::Launcher {

    LauncherLayer::LauncherLayer() :
        m_openProject(
            {
                .filters = { ".cxproj" }
            }
        ),
        m_createProject(
            {
                .filters = { "/" }
            }
        ) {}

    void LauncherLayer::OnAttach() {
        RecentProjects::Init();
        m_recentProjectsFiltered = RecentProjects::SearchProjects(m_searchFilter);
    }

    void LauncherLayer::OnGUI() {
        BeginViewport();
        RecentProjectList();
        EndViewport();
    }

    void LauncherLayer::BeginViewport() {
        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->Pos);
        ImGui::SetNextWindowSize(viewport->Size);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 15.0f, 15.0f });
        ImGui::Begin(
            "Launcher", nullptr,
            ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoCollapse |
            ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar |
            ImGuiWindowFlags_NoSavedSettings
        );
        ImGui::PopStyleVar(3);
    }

    void LauncherLayer::EndViewport() {
        ImGui::End();
    }

    bool LauncherLayer::TopActionBar() {
        bool changed = false;
        if (ImGui::BeginTable("TopBar", 3)) {
            ImGui::TableSetupColumn("Search", ImGuiTableColumnFlags_WidthStretch);
            ImGui::TableSetupColumn("Open", ImGuiTableColumnFlags_WidthFixed);
            ImGui::TableSetupColumn("New", ImGuiTableColumnFlags_WidthFixed);
            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            changed = Widgets::InputSearch("##Search", "Search recent projects...", m_searchFilter);
            ImGui::TableNextColumn();
            OpenProjectButton();
            ImGui::TableNextColumn();
            CreateProjectButton();
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 0, 0.0f });
            ImGui::EndTable();
            ImGui::PopStyleVar();
        }
        return changed;
    }

    void LauncherLayer::OpenProjectButton() {
        if (Widgets::Button("Open")) {
            ImGui::OpenPopup("Choose Project File");
        }
        if (Widgets::FileDialog("Choose Project File", m_openProject)) {
            if (m_openProject.GetSelection().IsOK()) {
                OpenProject(m_openProject.GetSelection().First());
            }
        }
    }

    void LauncherLayer::CreateProjectButton() {
        if (Widgets::Button("New Project")) {
            ImGui::OpenPopup("Create Project");
        }

        ImGui::SetNextWindowSizeConstraints({ 700.0f, -1.0f }, { INFINITY, -1.0f });
        if (ImGui::BeginPopupModal(
            "Create Project", nullptr,
            ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings
        )) {
            Widgets::InputFilePath(
                "Project Folder", m_createProject,
                "Select an empty directory. No new sub-folders will be created."
            );

            Widgets::InputText(
                "Project Name", m_projectName, ImGuiInputTextFlags_None,
                "This is only a display name, it will not affect any file names."
            );
            if (Widgets::InputTextWithAppend(
                "Project File", ".cxproj",
                m_projectFile, ImGuiInputTextFlags_None,
                "This file will be created inside the specified project \n"
                "folder. Do not include the file extension."
            )) {
                m_projectInfoLinked = false;
            }

            if (m_projectInfoLinked) {
                m_projectFile = StringUtils::ToLowercase(StringUtils::Replace(m_projectName, " ", "_"));
            }

            if (Widgets::BeginActions("Project Actions", 2)) {
                if (Widgets::Button("Cancel")) {
                    m_projectInfoLinked = true;
                    m_projectName = m_projectFile = "";
                    m_createProject.Reset();
                    ImGui::CloseCurrentPopup();
                }
                Widgets::NextAction();
                if (Widgets::Button(
                    "Create", !ValidationUtils::ValidateRequired(m_projectName) ||
                              !ValidationUtils::ValidateRequired(m_projectFile) ||
                              !m_createProject.GetSelection().IsOK())) {
                    if (m_projectFile.ends_with(".cxproj")) {
                        m_projectFile = m_projectFile.substr(0, m_projectFile.size() - StringView(".cxproj").size());
                    }
                    Path projectDirectory = m_createProject.GetSelection().First();
                    Calyx::Editor::ProjectGenerator generator(projectDirectory, m_projectFile + ".cxproj", m_projectName);
                    if (generator.Generate()) {
                        RecentProjects::AddProject(m_projectName, projectDirectory / (m_projectFile + ".cxproj"));
                        m_projectInfoLinked = true;
                        m_projectName = m_projectFile = "";
                        m_createProject.Reset();
                        m_updateProjectList = true;
                        ImGui::CloseCurrentPopup();
                    } else {
                        ImGui::OpenPopup("Project Creation Failed");
                    }
                }
                Widgets::InfoDialog(
                    "Project Creation Failed",
                    "Please select an empty project folder and retry the project creation."
                );
                Widgets::EndActions();
            }
            ImGui::EndPopup();
        }
    }

    void LauncherLayer::RecentProjectList() {
        if (TopActionBar() || m_updateProjectList) {
            m_recentProjectsFiltered = RecentProjects::SearchProjects(m_searchFilter);
            m_updateProjectList = false;
        }

        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 0, 8.0f });
        Widgets::Text(fmt::format("Recent Projects ({})", m_recentProjectsFiltered.size()));
        ImGui::PopStyleVar();

        ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, { 0.0f, 0.0f });
        ImGui::BeginChildFrame(1, { 0, 0 });
        ImGui::PopStyleVar(2);

        for (const auto& project: m_recentProjectsFiltered) {
            ProjectItem(project);
        }

        ImGui::EndChildFrame();
    }

    void LauncherLayer::ProjectItem(const ProjectMeta& meta) {
        static constexpr float s_itemHeight = 50.0f;
        static constexpr float s_itemPadding = 10.0f;

        auto cursor = ImGui::GetCursorPos();
        if (ImGui::Selectable(
            ("##" + meta.name).c_str(), false, ImGuiSelectableFlags_None,
            { 0, s_itemHeight + 2 * s_itemPadding }
        )) {
            OpenProject(meta.file);
        }
        if (ImGui::BeginPopupContextWindow(meta.file.c_str())) {
            if (ImGui::MenuItem("Remove from list")) {
                RecentProjects::RemoveProject(meta.file);
                m_updateProjectList = true;
            }
            ImGui::EndPopup();
        }
        auto finalCursor = ImGui::GetCursorPos();
        ImGui::SetCursorPos({ cursor.x + s_itemPadding, cursor.y + s_itemPadding });
        cursor = ImGui::GetCursorPos();
        auto pos = ImGui::GetCursorScreenPos();
        ImGui::GetWindowDrawList()->AddRectFilled(
            { pos.x, pos.y },
            { pos.x + s_itemHeight, pos.y + s_itemHeight },
            ImGui::ColorConvertFloat4ToU32({ meta.color.r, meta.color.g, meta.color.b, 1.0f }),
            3.0f
        );

        char first[2] = { meta.name[0], 0 };
        ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[CX_FONT_BOLD + CX_FONT_SIZE_LARGE]);
        auto letterSize = ImGui::CalcTextSize(first);
        ImGui::SetCursorPos(
            { cursor.x + (s_itemHeight - letterSize.x) / 2, cursor.y + (s_itemHeight - letterSize.y) / 2 }
        );
        Widgets::Text(first);
        ImGui::PopFont();

        ImGui::SetCursorPos({ cursor.x + s_itemHeight + s_itemPadding, cursor.y });
        Widgets::Text(meta.name);
        cursor = ImGui::GetCursorPos();
        ImGui::SetCursorPos({ cursor.x + s_itemHeight + 2 * s_itemPadding, cursor.y });
        Widgets::TextDisabled(meta.directory);

        ImGui::SetCursorPos(finalCursor);
    }

    void LauncherLayer::OpenProject(const String& file) {
        Path workingDir = PathUtils::Normalize(CX_EDITOR_PATH).parent_path();
        boost::process::child editor(CX_EDITOR_PATH, file, boost::process::start_dir(workingDir));
        editor.detach();
        Application::GetInstance().Close();
    }

}