#include "windows/SceneHierarchyPanel.h"

namespace Calyx::Editor {

    SceneHierarchyPanel::SceneHierarchyPanel(Scene* scene)
        : m_scene(scene) {}

    void SceneHierarchyPanel::Draw() {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
        ImGui::Begin("Scene Objects", nullptr, ImGuiWindowFlags_NoCollapse);
        ImGui::PopStyleVar();

        if (ImGui::BeginPopupContextWindow()) {
            if (ImGui::MenuItem("Create Empty")) {
                m_scene->CreateGameObject();
            }
            ImGui::EndPopup();
        }

        if (ImGui::BeginTable(
            "Scene Objects", 1,
            ImGuiTableFlags_BordersOuterH |
            ImGuiTableFlags_RowBg |
            ImGuiTableFlags_NoPadInnerX
        )) {
            ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_NoHide);
            ImGui::TableHeadersRow();
            for (auto* rootGameObject: m_scene->GetRootGameObjects()) {
                DrawGameObjectNode(rootGameObject);
            }
            ImGui::EndTable();
        }

        ImGui::End();
    }

    void SceneHierarchyPanel::DrawGameObjectNode(GameObject* gameObject) {
        ImGui::TableNextRow();
        ImGui::TableNextColumn();
        int flags = (gameObject->GetChildren().empty() ? ImGuiTreeNodeFlags_Leaf : 0) |
                    (m_selected == gameObject ? ImGuiTreeNodeFlags_Selected : 0) |
                    ImGuiTreeNodeFlags_OpenOnArrow |
                    ImGuiTreeNodeFlags_OpenOnDoubleClick |
                    ImGuiTreeNodeFlags_SpanFullWidth;
        bool open = ImGui::TreeNodeEx(gameObject, flags, "%s", gameObject->GetName().c_str());

        if (ImGui::IsItemClicked(ImGuiMouseButton_Left)) {
            m_selected = m_selected == gameObject ? nullptr : gameObject;
        }

        if (ImGui::IsItemClicked(ImGuiMouseButton_Right)) {
            m_selected = gameObject;
        }

        if (ImGui::BeginPopupContextItem()) {
            if (ImGui::MenuItem("Create Empty")) {
                auto* newObject = m_scene->CreateGameObject();
                newObject->SetParent(gameObject);
            }
            if (ImGui::MenuItem("Delete")) {
                m_scene->DeleteGameObject(gameObject);
                m_selected = nullptr;
            }
            ImGui::EndPopup();
        }

        if (open) {
            for (auto* child: gameObject->GetChildren()) {
                DrawGameObjectNode(child);
            }
            ImGui::TreePop();
        }
    }

}