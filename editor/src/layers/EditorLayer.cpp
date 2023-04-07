#include "layers/EditorLayer.h"
#include "scene/SceneManager.h"
#include "scene/serialization/SceneExporter.h"
#include "scene/serialization/SceneImporter.h"
#include "project/ProjectManager.h"
#include "background/BackgroundTaskManager.h"

namespace Calyx::Editor {

    EditorLayer::~EditorLayer() {
        SceneManager::UnloadCurrentScene();
    }

    void EditorLayer::OnAttach() {
        SceneManager::Init();
    }

    void EditorLayer::OnUpdate() {
        m_statsPanel.BeginFrame();
        SceneManager::GetInstance().Update();
        m_viewportPanel.OnUpdate();
        m_statsPanel.EndFrame();
    }

    void EditorLayer::OnGUI() {
        BeginDockspace();
        MenuBar();
        m_sceneHierarchyPanel.Draw();
        m_viewportPanel.OnGUI();
        m_statsPanel.Draw();
        m_inspectorPanel.Draw();
        m_contentBrowserPanel.Draw();
        BackgroundTaskManager::GetInstance().Draw();
        EndDockspace();
    }

    void EditorLayer::OnEvent(Event& event) {
        m_viewportPanel.OnEvent(event);
    }

    void EditorLayer::BeginDockspace() {
        // Setup parent window
        int windowFlags =
            ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking |
            ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse |
            ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->Pos);
        ImGui::SetNextWindowSize(viewport->Size);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.0f, 0.0f });
        ImGui::Begin("DockSpace", nullptr, windowFlags);
        ImGui::PopStyleVar(3);

        // Setup dock space
        ImGui::DockSpace(ImGui::GetID("DockSpace"), { 0.0f, 0.0f }, ImGuiDockNodeFlags_None);
    }

    void EditorLayer::MenuBar() {
        if (ImGui::BeginMenuBar()) {
            if (ImGui::BeginMenu("File")) {
                if (ImGui::MenuItem("New", "Ctrl+N")) {
                    SceneManager::LoadEmptyScene();
                }
                if (ImGui::MenuItem("Open...", "Ctrl+O")) {
                    SceneManager::LoadScene("./test.cxscene");
                }
                if (ImGui::MenuItem("Save", "Ctrl+S")) {
                    SceneExporter exporter(*SceneManager::GetCurrentScene());
                    exporter.Save("./test.cxscene");
                }
                if (ImGui::MenuItem("Exit", "Alt+F4")) {
                    Application::GetInstance().Close();
                }
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }
    }

    void EditorLayer::EndDockspace() {
        ImGui::End();
    }

}