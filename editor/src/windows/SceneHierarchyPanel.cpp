#include "windows/SceneHierarchyPanel.h"
#include "inspector/SelectionManager.h"
#include "scene/SceneManager.h"
#include "ui/Widgets.h"

#include <imgui.h>

namespace Calyx::Editor {

    void SceneHierarchyPanel::Draw() {
        m_scene = SceneManager::GetSimulationOrCurrentScene();
        if (m_scene == nullptr)
            return;

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
            ImGuiTableFlags_BordersH |
            ImGuiTableFlags_RowBg |
            ImGuiTableFlags_NoPadInnerX
        )) {
            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, { 10, 0 });
            ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_NoHide);
            ImGui::PopStyleVar();
            ImGui::TableHeadersRow();
            ImGui::PushStyleColor(ImGuiCol_Header, CX_COLOR_PRIMARY);
            ImGui::PushStyleColor(ImGuiCol_HeaderHovered, CX_COLOR_PRIMARY_HOVERED);
            for (auto* rootGameObject: m_scene->GetRootGameObjects()) {
                DrawGameObjectNode(rootGameObject);
            }
            ImGui::PopStyleColor(2);
            ImGui::EndTable();
        }

        ImGui::End();
    }

    void SceneHierarchyPanel::DrawGameObjectNode(GameObject* gameObject) {
        ImGui::TableNextRow();
        ImGui::TableNextColumn();
        bool isSelected = SelectionManager::IsSelected(gameObject);
        if (isSelected) {
            ImGui::PushStyleColor(ImGuiCol_HeaderHovered, CX_COLOR_PRIMARY);
        }
        int flags = (gameObject->GetChildren().empty() ? ImGuiTreeNodeFlags_Leaf : 0) |
                    (isSelected ? ImGuiTreeNodeFlags_Selected : 0) |
                    ImGuiTreeNodeFlags_OpenOnArrow |
                    ImGuiTreeNodeFlags_OpenOnDoubleClick |
                    ImGuiTreeNodeFlags_SpanFullWidth;
        bool open = ImGui::TreeNodeEx(gameObject, flags, "%s", gameObject->GetName().c_str());
        if (isSelected) {
            ImGui::PopStyleColor();
        }

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