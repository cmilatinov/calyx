#include "inspector/components/TransformComponentInspector.h"

#include <imgui.h>

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
            InspectorGUI::Property("Position");
            changed |= InspectorGUI::Vec3Control("pos", translation, 0.05f);

            InspectorGUI::Property("Rotation");
            changed |= InspectorGUI::Vec3Control("rot", rotation);

            InspectorGUI::Property("Scale");
            changed |= InspectorGUI::Vec3Control("scale", scale, 0.05f);

            InspectorGUI::EndPropertyTable();
        }

        if (changed) {
            transform.SetWorldMatrix(Math::ComposeTransform(translation, rotation, scale));
        }
    }

}