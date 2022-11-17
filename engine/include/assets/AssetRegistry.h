#pragma once

#include "assets/Asset.h"

#define CX_ASSET_REGISTRY_FRIEND()                          \
template<typename T>                                        \
friend T* ::Calyx::AssetRegistry::LoadAsset(const String&)

namespace Calyx {
    class Mesh;
}

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
        T* asset = T::Create(path);
        if (asset == nullptr) {
            return nullptr;
        }

        assets[path] = Scope<T>(asset);
        return asset;
    }

    template<typename T, typename ...Ts>
    void RegisterAssetType(Ts... extensions);

    Mesh* LoadScreenSpaceQuad();

    void Init();
    void UnloadAll();

}