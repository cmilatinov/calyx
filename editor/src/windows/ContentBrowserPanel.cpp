#include "windows/ContentBrowserPanel.h"
#include "ui/Widgets.h"
#include "project/ProjectManager.h"
#include "generators/ComponentGenerator.h"

#include <imgui.h>
#include <imgui_internal.h>

namespace Calyx::Editor {

    ContentBrowserPanel::ContentBrowserPanel()
        : m_objFileTexture(AssetRegistry::LoadAsset<Texture2D>("icons/block.png")),
          m_folderTexture(AssetRegistry::LoadAsset<Texture2D>("icons/folder-grey.png")),
          m_rootDirectory(PathUtils::Normalize(ProjectManager::GetAssetDirectory())),
          m_currentDirectory(m_rootDirectory) {}

    void ContentBrowserPanel::Draw() {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 8, 0 });
        ImGui::Begin("Content Browser", nullptr, ImGuiWindowFlags_MenuBar);
        ImGui::PopStyleVar();

        if (ImGui::BeginMenuBar()) {
            if (ImGui::MenuItem("<<")) {
                SetDirectory(m_currentDirectory.parent_path());
            }

            // Dynamically create the breadcrumbs
            Path tempPath = m_rootDirectory;
            ImGui::Text("/");
            for (auto& file: std::filesystem::relative(m_currentDirectory, m_rootDirectory)) {
                if (file.string() == ".")
                    continue;

                tempPath /= file;
                if (ImGui::MenuItem(file.string().c_str())) {
                    SetDirectory(tempPath);
                    break;
                }
                ImGui::Text("/");
            }

            ImGui::EndMenuBar();
        }

        ContextMenu();

        float lineHeight = ImGui::GetTextLineHeight();
        float cellSize = m_thumbnailSize + 2 * m_padding;
        float contentRegionWidth = ImGui::GetContentRegionAvail().x;
        ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, { m_padding, m_padding });
        ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[CX_FONT_REGULAR + CX_FONT_SIZE_SMALL]);
        if (ImGui::BeginTable("Files", std::max(1, (int)(contentRegionWidth / cellSize)), 0)) {
            for (const auto& entry: std::filesystem::directory_iterator(m_currentDirectory)) {
                auto& path = entry.path();
                String filename = path.filename().string();
                if (filename.ends_with(".meta"))
                    continue;

                ImGui::TableNextColumn();

                ImGui::PushStyleColor(ImGuiCol_Button, 0);
                ImGui::PushID(filename.c_str());
                auto textureID = entry.is_directory() ?
                                 reinterpret_cast<void*>(m_folderTexture->GetRendererID()) :
                                 reinterpret_cast<void*>(m_objFileTexture->GetRendererID());
                if (ImGui::ImageButton(textureID, { m_thumbnailSize, m_thumbnailSize }) && entry.is_directory()) {
                    SetDirectory(entry.path());
                }
                ImGui::PopID();
                ImGui::PopStyleColor();

                auto contentRegionX = ImGui::GetContentRegionAvail().x;
                auto textSize = ImGui::CalcTextSize(filename.c_str());
                auto textSizeX = std::min(textSize.x, m_thumbnailSize);
                auto* window = ImGui::GetCurrentWindow();
                auto cursorPos = window->DC.CursorPos;
                float textPosX = cursorPos.x + (m_padding + m_thumbnailSize - textSizeX) * 0.5f;
                float textPosMaxX = cursorPos.x + contentRegionX;
                ImGui::RenderTextEllipsis(
                    window->DrawList,
                    { textPosX, cursorPos.y },
                    { textPosMaxX, cursorPos.y + lineHeight },
                    textPosMaxX, textPosMaxX,
                    filename.c_str(), nullptr, nullptr
                );
                ImGui::Dummy({ textSizeX, textSize.y });

            }
            ImGui::EndTable();
        }
        ImGui::PopFont();
        ImGui::PopStyleVar();

        ImGui::End();
    }

    void ContentBrowserPanel::ContextMenu() {
        int openPopup = 0;
        if (ImGui::BeginPopupContextWindow()) {
            if (ImGui::BeginMenu("New")) {
                if (ImGui::MenuItem("Folder")) {
                    openPopup = 1;
                }
                if (ImGui::MenuItem("Component Class (C++)")) {
                    openPopup = 2;
                }
                ImGui::EndMenu();
            }
            ImGui::EndPopup();
        }
        switch (openPopup) {
            case 1:
                Widgets::OpenPopup("New Folder");
                break;
            case 2:
                Widgets::OpenPopup("New Component");
                break;
            default:
                break;
        }
        NewFolder();
        NewComponent();
    }

    void ContentBrowserPanel::NewFolder() {
        if (Widgets::BeginPopupModal("New Folder")) {
            Widgets::InputText("Name", m_folderName);
            Widgets::BeginActions("Actions", 2);
            if (Widgets::Button("Cancel")) {
                ResetNewFolder();
                ImGui::CloseCurrentPopup();
            }
            Widgets::NextAction();
            if (Widgets::Button("Create", !ValidationUtils::ValidateRequired(m_folderName))) {
                Path directory = m_currentDirectory / m_folderName;
                FileSystem::create_directories(directory);
                ResetNewFolder();
                ImGui::CloseCurrentPopup();
            }
            Widgets::EndActions();
            Widgets::EndPopup();
        }
    }

    void ContentBrowserPanel::ResetNewFolder() {
        m_folderName = "";
    }

    void ContentBrowserPanel::NewComponent() {
        if (Widgets::BeginPopupModal("New Component")) {
            Widgets::InputText("Namespace", m_componentNamespace);
            Widgets::InputText("Name", m_componentName);
            Widgets::BeginActions("Actions", 2);
            if (Widgets::Button("Cancel")) {
                ImGui::CloseCurrentPopup();
                ResetNewComponent();
            }
            Widgets::NextAction();
            if (Widgets::Button(
                "Create",
                !(StringUtils::RegexMatch(m_componentName, "^[a-zA-Z]+$") ||
                  StringUtils::RegexMatch(m_componentNamespace, "^([a-zA-Z]+::)+[a-zA-Z]+$")))) {
                ComponentGenerator generator(
                    ProjectManager::GetAssetDirectory(),
                    m_currentDirectory,
                    m_componentName,
                    m_componentNamespace
                );
                generator.Generate();
                ResetNewComponent();
                ImGui::CloseCurrentPopup();
            }
            Widgets::EndActions();
            Widgets::EndPopup();
        }
    }

    void ContentBrowserPanel::ResetNewComponent() {
        m_componentName = "";
        m_componentNamespace = "Calyx";
    }

    void ContentBrowserPanel::SetDirectory(const Path& path) {
        auto relativePath = FileSystem::relative(path, m_rootDirectory).string();
        if (relativePath.starts_with(".."))
            return;

        m_currentDirectory = PathUtils::Normalize(path);
    }

}