#include "assets/AssetRegistry.h"

#include "assets/Mesh.h"
#include "render/objects/Shader.h"
#include "render/objects/Texture2D.h"

namespace Calyx::AssetRegistry {

    Map<String, AssetT>& GetAssetTypes() {
        static Map<String, AssetT> assetTypes;
        return assetTypes;
    }

    Map<String, Scope<IAsset>>& GetAssets() {
        static Map<String, Scope<IAsset>> assets;
        return assets;
    }

    template<typename T>
    AssetT AssetType() {
        return typeid(T).hash_code();
    }

    template<typename T, typename ...Ts>
    void RegisterAssetType(Ts... extensions) {
        auto& assetTypes = GetAssetTypes();
        List<String> exts = { extensions... };
        for (const auto& ext: exts) {
            assetTypes[ext] = AssetType<T>();
        }
    }

    Mesh* LoadScreenSpaceQuad() {
        auto& assets = GetAssets();
        assets["quad"] = CreateScope<Mesh>();
        auto* mesh = reinterpret_cast<Mesh*>(assets["quad"].get());
        mesh->SetIndices(
            {
                0, 1, 2,
                1, 0, 3
            }
        );
        mesh->SetVertices(
            {
                vec3(-1, -1, 0),
                vec3(1, 1, 0),
                vec3(-1, 1, 0),
                vec3(1, -1, 0)
            }
        );
        return mesh;
    }

    void Init() {
        RegisterAssetType<Mesh>(".obj", ".fbx", ".3ds", ".blend", ".ply");
        RegisterAssetType<Shader>(".glsl");
        RegisterAssetType<Texture2D>(".png", ".jpg", ".bmp");
    }

    void UnloadAll() {
        GetAssets().clear();
    }

}