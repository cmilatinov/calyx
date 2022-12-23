#pragma once

#include "assets/Asset.h"

namespace Calyx::Editor {

    class InspectorGUI {

    public:
        static bool GameAssetControl(const String& name, AssetT assetType, Ref<IAsset>& value);

        static bool TextControl(const String& name, String& value);
        static bool Vec3Control(const String& name, vec3& value, float speed = 1.0f);
        static bool Vec2Control(const String& name, vec2& value, float speed = 1.0f);
        static bool SliderControl(const String& name, float& value, float min = 0.0f, float max = 0.0f);

        static bool BeginPropertyTable(const String& name);
        static void EndPropertyTable();

        static void Property(const String& label);

    private:
        static String s_assetSearch;
        static List<AssetRegistry::AssetMeta> s_assetList;
        static int s_assetSelected;

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