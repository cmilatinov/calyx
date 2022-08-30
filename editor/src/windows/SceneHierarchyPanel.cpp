#include "windows/SceneHierarchyPanel.h"

namespace Calyx::Editor {

    SceneHierarchyPanel::SceneHierarchyPanel(Scene* scene)
        : m_scene(scene) {}

    void SceneHierarchyPanel::Draw() {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 10));
        ImGui::Begin("Scene Objects", nullptr, ImGuiWindowFlags_NoCollapse);
        ImGui::PopStyleVar();

        if (ImGui::BeginPopupContextWindow()) {
            if (ImGui::MenuItem("Create Empty")) {
                m_scene->CreateGameObject();
            }
            ImGui::EndPopup();
        }

        ImGui::PushStyleVar(ImGuiStyleVar_IndentSpacing, 15);
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
        for (auto* rootGameObject: m_scene->GetRootGameObjects()) {
            DrawGameObjectNode(rootGameObject);
        }
        ImGui::PopStyleVar(2);

        ImGui::End();
    }

    void SceneHierarchyPanel::DrawGameObjectNode(GameObject* gameObject) {
        int flags = (gameObject->GetChildren().empty() ? ImGuiTreeNodeFlags_Leaf : 0) |
                    (m_selected == gameObject ? ImGuiTreeNodeFlags_Selected : 0) |
                    ImGuiTreeNodeFlags_OpenOnArrow |
                    ImGuiTreeNodeFlags_OpenOnDoubleClick |
                    ImGuiTreeNodeFlags_SpanFullWidth;
        bool open = ImGui::TreeNodeEx(gameObject, flags, "%s", gameObject->GetName().c_str());

        if (ImGui::BeginPopupContextItem()) {
            if (ImGui::MenuItem("Create Empty")) {
                auto* newObject = m_scene->CreateGameObject();
                newObject->SetParent(gameObject);
            }
            if (ImGui::MenuItem("Delete")) {

            }
            ImGui::EndPopup();
        }

        if (ImGui::IsItemClicked()) {
            m_selected = m_selected == gameObject ? nullptr : gameObject;
        }

        if (open) {
            for (auto* child: gameObject->GetChildren()) {
                DrawGameObjectNode(child);
            }
            ImGui::TreePop();
        }
    }

}