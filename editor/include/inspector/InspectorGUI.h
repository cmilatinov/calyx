#pragma once

#include "assets/Asset.h"

namespace Calyx {
    class GameObject;
}

namespace Calyx::Editor {

    class InspectorGUI {

    public:
        static bool ComponentPicker(GameObject* gameObject, const Set<AssetType>& excluded);

        static bool GameAssetControl(const String& name, AssetType assetType, Ref<IAsset>& value);

        static bool TextControl(const String& name, String& value, bool readonly = false);
        static bool Vec3Control(const String& name, vec3& value, float speed = 1.0f);
        static bool Vec2Control(const String& name, vec2& value, float speed = 1.0f);
        static bool FloatControl(const String& name, float& value, float speed = 1.0f, float min = 0.0f, float max = 0.0f);

        static bool BeginPropertyTable(const String& name);
        static void EndPropertyTable();

        static void Property(const String& label);

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