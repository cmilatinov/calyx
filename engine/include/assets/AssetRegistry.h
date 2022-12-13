#pragma once

#include <efsw/efsw.hpp>

#include "assets/Asset.h"

#define CX_ASSET_REGISTRY_FRIEND()                          \
friend class ::Calyx::AssetRegistry;

namespace Calyx {

    class AssetRegistry : public efsw::FileWatchListener {
    CX_SINGLETON(AssetRegistry);

    private:
        struct AssetSearchPath {
            Path path;
            efsw::WatchID watch;
            List<String> assets;
        };

        struct AssetMeta {
            AssetT type;
            String name;
            Path path;
        };

        template<typename T>
        class AssetDeleter {
            static_assert(std::is_base_of_v<IAsset, T>, "T must be an asset type!");

        public:
            explicit AssetDeleter(String name)
                : m_name(std::move(name)) {}

            void operator()(const T* ptr) {
                delete ptr;
                AssetRegistry::GetInstance().m_loadedAssets.erase(m_name);
            }

        private:
            String m_name;

        };

    public:
        template<typename T>
        static inline AssetT AssetType() {
            return typeid(T).hash_code();
        }

        template<typename T, typename ...Args>
        static inline Ref<T> CreateAsset(const String& name, Args... args) {
            return s_instance->_CreateAsset<T, Args...>(name, args...);
        }

        template<typename T>
        static inline Ref<T> LoadAsset(const String& path) {
            return s_instance->_LoadAsset<T>(path);
        }

        template<typename T, typename ...Ext>
        static inline void RegisterAssetType(Ext... extensions) {
            return s_instance->_RegisterAssetType<T, Ext...>(extensions...);
        }

        CX_SINGLETON_EXPOSE_METHOD(_UnloadAll, void UnloadAll());
        CX_SINGLETON_EXPOSE_METHOD(_AddSearchPath, void AddSearchPath(const String& path), path);
        CX_SINGLETON_EXPOSE_METHOD(_RemoveSearchPath, void RemoveSearchPath(const String& path), path);

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
        String NormalizePath(const String& path);

        bool FindAssetType(const Path& file, AssetT& outType);
        bool ExpectAssetType(const Path& file, AssetT expectedType);

        bool FindAssetFile(const String& asset, Path& outFilePath);
        void BuildAssetMeta(AssetSearchPath& searchPath);
        void ClearAssetMeta(AssetSearchPath& searchPath);

        void InitAssetTypes();

    private:
        template<typename T, typename ...Args>
        Ref<T> _CreateAsset(const String& name, Args... args) {
            static_assert(std::is_base_of_v<IAsset, T>, "T must be an asset type!");
            String assetName = NormalizePath(name);
            auto ref = CreateRef<T>(std::forward<Args>(args)...);
            m_loadedAssets[assetName] = WeakRef<IAsset>(ref);
            return ref;
        }

        template<typename T>
        Ref<T> _LoadAsset(const String& name) {
            // Asset name & type
            String assetName = NormalizePath(name);
            AssetT assetType = AssetType<T>();

            // Asset already loaded
            auto loadedAsset = m_loadedAssets.find(assetName);
            if (loadedAsset != m_loadedAssets.end()) {
                auto assetPtr = loadedAsset->second.lock();
                return assetPtr->GetAssetType() == assetType ?
                       std::dynamic_pointer_cast<T>(assetPtr) :
                       Ref<T>();
            }

            // Check file exists
            Path assetFile;
            if (!FindAssetFile(assetName, assetFile))
                return Ref<T>();

            // Check extension matches asset type
            String ext = assetFile.extension().string();
            auto extEntry = m_assetTypes.find(ext);
            if (!ExpectAssetType(assetFile, assetType))
                return Ref<T>();

            // Load asset from file
            T* asset = T::Create(assetFile.string());
            if (asset == nullptr)
                return Ref<T>();

            // Return ref
            Ref<T> ref(asset, AssetDeleter<T>(assetName));
            m_loadedAssets[assetName] = WeakRef<IAsset>(ref);
            return ref;
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

    private:
        Scope<efsw::FileWatcher> m_assetWatcher;

        List<AssetSearchPath> m_searchPaths{};

        Map<String, AssetT> m_assetTypes{};
        Map<String, AssetMeta> m_assetMetaMap{};
        Map<String, WeakRef<IAsset>> m_loadedAssets{};

    };

}