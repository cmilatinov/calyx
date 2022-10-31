#include "inspector/InspectorGUI.h"
#include <imgui_internal.h>

namespace Calyx::Editor {

    bool InspectorGUI::DrawVec3Control(const String& name, vec3& value, float speed) {
        return DragFloatN_Colored(name.c_str(), glm::value_ptr(value), 3, speed);
    }

    bool InspectorGUI::DrawVec2Control(const String& name, vec2& value, float speed) {
        return DragFloatN_Colored(name.c_str(), glm::value_ptr(value), 2, speed);
    }

    bool InspectorGUI::BeginPropertyTable(const String& name) {
        bool visible = ImGui::BeginTable(
            name.c_str(), 2, ImGuiTableFlags_BordersInnerV | ImGuiTableFlags_Resizable
        );
        return visible;
    }

    void InspectorGUI::EndPropertyTable() {
        ImGui::EndTable();
    }

    void InspectorGUI::PropertyRow(const String& label) {
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