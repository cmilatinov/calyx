#pragma once

#include "assets/Asset.h"
#include "assets/Mesh.h"
#include "render/objects/Shader.h"

namespace Calyx::AssetRegistry {

    Map<String, AssetT>& GetAssetTypes();
    Map<String, Scope<IAsset>>& GetAssets();

    template<typename T>
    AssetT AssetType();

    template<typename T>
    T* LoadAsset(const String& path) {
        auto& assets = GetAssets();
        auto& assetTypes = GetAssetTypes();

        // Asset already loaded (check asset is correct type)
        auto entry = assets.find(path);
        if (entry != assets.end() && entry->second->GetAssetType() == AssetType<T>()) {
            return reinterpret_cast<T*>(entry->second.get());
        } else if (entry != assets.end()) {
            return nullptr;
        }

        // Check extension matches asset type
        String ext = Path(path).extension().string();
        auto extEntry = assetTypes.find(ext);
        if (extEntry == assetTypes.end() ||
            extEntry->second != AssetType<T>()) {
            return nullptr;
        }

        // Load asset from file
        assets[path] = CreateScope<T>();
        if (!assets[path]->Load(path)) {
            assets.erase(path);
            return nullptr;
        }

        return reinterpret_cast<T*>(assets[path].get());
    }

    template<>
    Shader* LoadAsset<Shader>(const String& path);

    template<typename T, typename ...Ts>
    void RegisterAssetType(Ts... extensions);

    Mesh* LoadScreenSpaceQuad();

    void Init();
    void UnloadAll();

}