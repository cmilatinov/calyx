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
            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, { 10, 0 });
            ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_NoHide);
            ImGui::PopStyleVar();
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
        bool isSelected = SelectionManager::IsSelected(gameObject);
        int flags = (gameObject->GetChildren().empty() ? ImGuiTreeNodeFlags_Leaf : 0) |
                    (isSelected ? ImGuiTreeNodeFlags_Selected : 0) |
                    ImGuiTreeNodeFlags_OpenOnArrow |
                    ImGuiTreeNodeFlags_OpenOnDoubleClick |
                    ImGuiTreeNodeFlags_SpanFullWidth;
        bool open = ImGui::TreeNodeEx(gameObject, flags, "%s", gameObject->GetName().c_str());

        if (ImGui::IsItemClicked(ImGuiMouseButton_Left)) {
            if (isSelected)
                SelectionManager::SetCurrentSelection(Selection());
            else
                SelectionManager::SetCurrentSelection(Selection::Create<GameObject>().AddItem(gameObject));
        }

        if (ImGui::IsItemClicked(ImGuiMouseButton_Right)) {
            SelectionManager::SetCurrentSelection(Selection::Create<GameObject>().AddItem(gameObject));
        }

        if (ImGui::BeginPopupContextItem()) {
            if (ImGui::MenuItem("Create Empty")) {
                auto* newObject = m_scene->CreateGameObject();
                newObject->SetParent(gameObject);
            }
            if (ImGui::MenuItem("Delete")) {
                m_scene->DeleteGameObject(gameObject);
                SelectionManager::SetCurrentSelection(Selection());
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