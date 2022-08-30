#include "assets/AssetRegistry.h"

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

    template<>
    Shader* LoadAsset<Shader>(const String& path) {
        auto& assets = GetAssets();
        auto& assetTypes = GetAssetTypes();

        // Asset already loaded (check asset is correct type)
        auto entry = assets.find(path);
        if (entry != assets.end() && entry->second->GetAssetType() == AssetType<Shader>()) {
            return reinterpret_cast<Shader*>(entry->second.get());
        } else if (entry != assets.end()) {
            return nullptr;
        }

        // Check extension matches asset type
        String ext = Path(path).extension().string();
        auto extEntry = assetTypes.find(ext);
        if (extEntry == assetTypes.end() ||
            extEntry->second != AssetType<Shader>()) {
            return nullptr;
        }

        // Load shader from file
        assets[path] = Shader::Create(path);
        return reinterpret_cast<Shader*>(assets[path].get());
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
        mesh->SetIndices({
            0, 1, 2,
            1, 0, 3
        });
        mesh->SetVertices({
            vec3(-1, -1, 0),
            vec3(1, 1, 0),
            vec3(-1, 1, 0),
            vec3(1, -1, 0)
        });
        return mesh;
    }

    void Init() {
        RegisterAssetType<Mesh>(".obj", ".fbx", ".3ds", ".blend", ".ply");
        RegisterAssetType<Shader>(".glsl");
    }

    void UnloadAll() {
        GetAssets().clear();
    }

}