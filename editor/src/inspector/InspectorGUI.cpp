#include "inspector/InspectorGUI.h"

#include "ecs/GameObject.h"

#include <imgui.h>
#include <imgui_stdlib.h>
#include <imgui_internal.h>

namespace Calyx::Editor {

    bool InspectorGUI::ComponentPicker(GameObject* gameObject, const Set<AssetType>& excluded) {
        static String s_componentSearch{};
        static List<AssetRegistry::AssetMeta> s_componentList{};

        if (ImGui::Button("Add Component")) {
            ImGui::OpenPopup("##ComonentPicker");
        }

        if (ImGui::BeginPopup("##ComonentPicker")) {
            if (ImGui::IsWindowAppearing()) {
                s_componentList.clear();
                AssetRegistry::SearchComponents(s_componentSearch, excluded, s_componentList);
                ImGui::SetKeyboardFocusHere();
            }

            if (ImGui::InputText("##ComponentSearch", &s_componentSearch, ImGuiInputTextFlags_AutoSelectAll)) {
                s_componentList.clear();
                AssetRegistry::SearchComponents(s_componentSearch, excluded, s_componentList);
            }

            for (const auto& component: s_componentList) {
                if (ImGui::Selectable(component.displayName.c_str(), false)) {
                    gameObject->AddComponent(entt::resolve(component.type));
                }
            }

            ImGui::EndPopup();
        }
        return false;
    }

    bool InspectorGUI::GameAssetControl(const String& name, AssetType assetType, Ref<IAsset>& value) {
        static String s_assetSearch{};
        static List<AssetRegistry::AssetMeta> s_assetList{};

        auto displayName = AssetRegistry::GetAssetDisplayName(value);
        if (ImGui::BeginCombo("##AssetSelect", displayName.c_str())) {
            if (ImGui::IsWindowAppearing()) {
                s_assetList.clear();
                AssetRegistry::SearchAssets(assetType, s_assetSearch, s_assetList);
                ImGui::SetKeyboardFocusHere();
            }

            ImGui::SetNextItemWidth(-FLT_MIN);
            if (ImGui::InputText("##AssetSearch", &s_assetSearch, ImGuiInputTextFlags_AutoSelectAll)) {
                s_assetList.clear();
                AssetRegistry::SearchAssets(assetType, s_assetSearch, s_assetList);
            }

            if (ImGui::Selectable("None", !value)) {
                value = Ref<IAsset>();
            }

            for (const auto& asset: s_assetList) {
                auto id = AssetRegistry::GetAssetID(value);
                if (ImGui::Selectable(asset.displayName.c_str(), asset.id == id)) {
                    value = AssetRegistry::LoadAsset(asset.id);
                }
            }
            ImGui::EndCombo();
        }

        return false;
    }

    bool InspectorGUI::TextControl(const String& name, String& value, bool readonly) {
        ImGuiInputTextFlags flags = ImGuiInputTextFlags_AutoSelectAll;
        if (readonly)
            flags |= ImGuiInputTextFlags_ReadOnly;
        return ImGui::InputText(("##" + name).c_str(), &value, flags);
    }

    bool InspectorGUI::Vec3Control(const String& name, vec3& value, float speed) {
        return DragFloatN_Colored(name.c_str(), glm::value_ptr(value), 3, speed);
    }

    bool InspectorGUI::Vec2Control(const String& name, vec2& value, float speed) {
        return DragFloatN_Colored(name.c_str(), glm::value_ptr(value), 2, speed);
    }

    bool InspectorGUI::FloatControl(const String& name, float& value, float speed, float min, float max) {
        return ImGui::DragFloat(("##" + name).c_str(), &value, speed, min, max);
    }

    bool InspectorGUI::BeginPropertyTable(const String& name) {
        bool visible = ImGui::BeginTable(
            name.c_str(), 2, ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_BordersInnerV | ImGuiTableFlags_Resizable
        );
        ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableSetupColumn("Control", ImGuiTableColumnFlags_WidthStretch);
        return visible;
    }

    void InspectorGUI::EndPropertyTable() {
        ImGui::EndTable();
    }

    void InspectorGUI::Property(const String& label) {
        ImGui::TableNextRow();
        ImGui::TableNextColumn();
        ImGui::Text(label.c_str());
        ImGui::TableNextColumn();
        ImGui::SetNextItemWidth(-FLT_MIN);
    }

    bool InspectorGUI::DragFloatN_Colored(
        const char* label, float* v, int components, float v_speed, float v_min, float v_max,
        const char* display_format, float power
    ) {
        ImGuiWindow* window = ImGui::GetCurrentWindow();
        if (window->SkipItems)
            return false;

        ImGuiContext& g = *GImGui;
        bool value_changed = false;
        ImGui::BeginGroup();
        ImGui::PushID(label);
        ImGui::PushMultiItemsWidths(components, ImGui::CalcItemWidth());
        for (int i = 0; i < components; i++) {
            static const ImU32 colors[] = {
                0xFF261ACC, // red
                0xFF33CC33, // green
                0xFFCC401A  // blue
            };

            ImGui::PushID(i);
            value_changed |= ImGui::DragFloat("##v", &v[i], v_speed, v_min, v_max, display_format, power);

            const ImVec2 min = ImGui::GetItemRectMin();
            const ImVec2 max = ImGui::GetItemRectMax();
            const float lineWidth = 6.0f;
            window->DrawList->AddRectFilled(
                { min.x, min.y },
                { min.x + lineWidth, max.y },
                colors[i]
            );

            ImGui::SameLine(0, g.Style.ItemInnerSpacing.x);
            ImGui::PopID();
            ImGui::PopItemWidth();
        }
        ImGui::PopID();
        ImGui::EndGroup();

        return value_changed;
    }

}