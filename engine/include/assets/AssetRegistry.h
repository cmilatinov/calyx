#pragma once

#include <efsw/efsw.hpp>

#include "assets/Asset.h"

#define CX_ASSET_REGISTRY_FRIEND()                          \
friend class ::Calyx::AssetRegistry;

namespace Calyx {

    class AssetRegistry : public efsw::FileWatchListener {
    CX_SINGLETON(AssetRegistry);

    public:
        template<typename T>
        static inline AssetT AssetType() {
            return typeid(T).hash_code();
        }

        template<typename T, typename ...Args>
        static inline WeakRef<T> CreateAsset(const String& name, Args... args) {
            return s_instance->_CreateAsset<T, Args...>(name, args...);
        }

        template<typename T>
        static inline WeakRef<T> LoadAsset(const String& path) {
            return s_instance->_LoadAsset<T>(path);
        }

        template<typename T, typename ...Ext>
        static inline void RegisterAssetType(Ext... extensions) {
            return s_instance->_RegisterAssetType<T, Ext...>(extensions...);
        }

        CX_SINGLETON_EXPOSE_METHOD(_UnloadAll, void UnloadAll());
        CX_SINGLETON_EXPOSE_METHOD(_AddSearchPath, void AddSearchPath(const String& path), path);
        CX_SINGLETON_EXPOSE_METHOD(_RemoveSearchPath, void RemoveSearchPath(const String& path), path);
        CX_SINGLETON_EXPOSE_METHOD(_GetRootAssetPath, Path GetRootAssetPath());

    public:
        AssetRegistry();

        void handleFileAction(
            efsw::WatchID watchID,
            const std::string& dir,
            const std::string& filename,
            efsw::Action action,
            std::string oldFilename
        ) override;

    private:
        String FindAssetFile(const String& asset);

    private:
        template<typename T, typename ...Args>
        WeakRef<T> _CreateAsset(const String& name, Args... args) {
            static_assert(std::is_base_of_v<IAsset, T>, "T must be an asset type!");
            auto ref = CreateRef<T>(std::forward<Args>(args)...);
            m_loadedAssets[name] = ref;
            return ref;
        }

        template<typename T>
        WeakRef<T> _LoadAsset(const String& name) {
            // Asset already loaded
            auto loadedAsset = _GetAsset<T>(name);
            if (!loadedAsset.expired())
                return loadedAsset;

            // Check file exists
            String filepath = FindAssetFile(name);
            Path file = Path(filepath);
            if (!FileSystem::exists(file) || !FileSystem::is_regular_file(file))
                return WeakRef<T>();

            // Check extension matches asset type
            String ext = file.extension().string();
            auto extEntry = m_assetTypes.find(ext);
            if (extEntry == m_assetTypes.end() ||
                extEntry->second != AssetType<T>())
                return WeakRef<T>();

            // Load asset from file
            T* asset = T::Create(filepath);
            if (asset == nullptr)
                return WeakRef<T>();

            // Return ref
            Ref<T> ref(asset);
            m_loadedAssets[name] = ref;
            return ref;
        }

        template<typename T>
        WeakRef<T> _GetAsset(const String& name) {
            auto asset = m_loadedAssets.find(name);
            if (asset == m_loadedAssets.end() || asset->second->GetAssetType() != AssetType<T>())
                return WeakRef<T>();
            return std::dynamic_pointer_cast<T>(asset->second);
        }

        template<typename T, typename ...Ext>
        void _RegisterAssetType(Ext... extensions) {
            List<String> exts = { extensions... };
            for (const auto& ext: exts) {
                m_assetTypes[ext] = AssetType<T>();
            }
        }

        void _UnloadAll();

        void _AddSearchPath(const String& path);

        void _RemoveSearchPath(const String& path);

        Path _GetRootAssetPath() const { return m_rootAssetDirectory; }

    private:
        Scope<efsw::FileWatcher> m_assetWatcher;
        Path m_rootAssetDirectory;
        OrderedSet<Path> m_searchPaths = { "assets" };
        Map<String, AssetT> m_assetTypes{};
        Map<String, Ref<IAsset>> m_loadedAssets{};

    };

}