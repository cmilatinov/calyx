#include "inspector/components/TransformComponentInspector.h"

namespace Calyx::Editor {

    void TransformComponentInspector::CX_ON_INSPECTOR_CONTEXT_GUI(TransformComponent& tc) {
        if (ImGui::MenuItem("Reset")) {
            tc.GetTransform().Reset();
        }
    }

    void TransformComponentInspector::CX_ON_INSPECTOR_GUI(TransformComponent& tc) {
        auto& transform = tc.GetTransform();

        vec3 translation, rotation, scale;
        Math::DecomposeTransform(transform.GetMatrix(), translation, rotation, scale);

        bool changed = false;
        if (InspectorGUI::BeginPropertyTable("Transform")) {
            if (ImGui::BeginPopupContextWindow("Transform Popup")) {
                CX_ON_INSPECTOR_CONTEXT_GUI(tc);
                ImGui::EndPopup();
            }

            InspectorGUI::PropertyRow("Position");
            changed |= InspectorGUI::DrawVec3Control("pos", translation, 0.05f);

            InspectorGUI::PropertyRow("Rotation");
            changed |= InspectorGUI::DrawVec3Control("rot", rotation);

            InspectorGUI::PropertyRow("Scale");
            changed |= InspectorGUI::DrawVec3Control("scale", scale, 0.05f);

            InspectorGUI::EndPropertyTable();
        }

        if (changed) {
            transform.SetWorldMatrix(Math::ComposeTransform(translation, rotation, scale));
        }
    }

}