#pragma once

#include <imgui.h>

namespace Calyx::Editor {

    class InspectorGUI {

    public:
        static bool DrawVec3Control(const String& name, vec3& value, float speed = 1.0f);
        static bool DrawVec2Control(const String& name, vec2& value, float speed = 1.0f);

        static bool BeginPropertyTable(const String& name);
        static void EndPropertyTable();

        static void PropertyRow(const String& label);

    private:
        static bool DragFloatN_Colored(
            const char* label,
            float* v,
            int components,
            float v_speed = 1.0f,
            float v_min = 0.0f,
            float v_max = 0.0f,
            const char* display_format = "%.3f",
            float power = 1.0f
        );

    };

}