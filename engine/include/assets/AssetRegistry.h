#pragma once

#include <efsw/efsw.hpp>

#include "assets/Asset.h"

#define CX_ASSET_REGISTRY_FRIEND()                          \
friend class ::Calyx::AssetRegistry;

namespace Calyx::Editor {
    class AssemblyBuilder;
}

namespace Calyx {

    class AssetRegistry : public efsw::FileWatchListener {
    CX_SINGLETON(AssetRegistry);

        friend class Editor::AssemblyBuilder;

    public:
        struct AssetMeta {
            UUID id;
            AssetType type;
            String name;
            String displayName;
            Path path;
            bool isComponent = false;
            const IAsset* ptr = nullptr;
        };

    private:
        struct AssetSearchPath {
            Path path;
            efsw::WatchID watch;
            List<UUID> assets;
        };

        struct RegisteredAssetType {
            AssetType type;
            IAsset* (* loadAssetFn)(const String&);
        };

        template<typename T>
        class AssetDeleter {
            static_assert(std::is_base_of_v<IAsset, T>, "T must be an asset type!");

        public:
            explicit AssetDeleter(const UUID& id) : m_id(id) {}

            void operator()(const T* ptr) {
                auto& registry = AssetRegistry::GetInstance();
                registry.m_loadedAssets.erase(m_id);
                registry.m_assetPointers_IDs.erase(static_cast<const IAsset*>(ptr));
                CX_MAP_FIND(registry.m_assetMeta, m_id, i_assetMeta) {
                    i_assetMeta->second.ptr = nullptr;
                }
                delete ptr;
            }

        private:
            UUID m_id;

        };

    public:
        template<typename T>
        static inline AssetType GetAssetType() {
            return entt::resolve<T>().id();
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

        CX_SINGLETON_EXPOSE_METHOD(_LoadAsset, Ref<IAsset> LoadAsset(const UUID& id), id);
        CX_SINGLETON_EXPOSE_METHOD(_UnloadAll, void UnloadAll());
        CX_SINGLETON_EXPOSE_METHOD(_AddSearchPath, void AddSearchPath(const String& path), path);
        CX_SINGLETON_EXPOSE_METHOD(_RemoveSearchPath, void RemoveSearchPath(const String& path), path);
        CX_SINGLETON_EXPOSE_METHOD(
            _SearchAssets,
            void SearchAssets(AssetType assetType, const String& query, List<AssetMeta>& outList),
            assetType, query, outList
        );
        CX_SINGLETON_EXPOSE_METHOD(
            _SearchComponents,
            void SearchComponents(const String& query, const Set<AssetType>& excluded, List<AssetMeta>& outList),
            query, excluded, outList
        );

        CX_SINGLETON_EXPOSE_METHOD(_GetAssetID, UUID GetAssetID(const IAsset* ptr), ptr);
        CX_SINGLETON_EXPOSE_METHOD(_GetAssetID, UUID GetAssetID(const Ref<IAsset>& ref), ref);
        CX_SINGLETON_EXPOSE_METHOD(_GetAssetDisplayName, String GetAssetDisplayName(const IAsset* ptr), ptr);
        CX_SINGLETON_EXPOSE_METHOD(_GetAssetDisplayName, String GetAssetDisplayName(const Ref<IAsset>& ref), ref);
        CX_SINGLETON_EXPOSE_METHOD(_GetAssetMeta, const AssetMeta* GetAssetMeta(const IAsset* ptr), ptr);
        CX_SINGLETON_EXPOSE_METHOD(_GetAssetMeta, const AssetMeta* GetAssetMeta(const Ref<IAsset>& ref), ref);

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

        bool FindAssetType(const Path& file, AssetType& outType);
        bool ExpectAssetType(const Path& file, AssetType expectedType);

        bool FindAssetFile(const String& asset, Path& outFilePath);
        void BuildAssetMeta(AssetSearchPath& searchPath);
        void ClearAssetMeta(AssetSearchPath& searchPath);

        Path GetMetaFile(const Path& assetFile);
        bool LoadMetaFile(const Path& assetFile, json& meta);
        void WriteMetaFile(const Path& assetFile, const AssetMeta& meta);

        void InitAssetTypes();

    private:
        template<typename T, typename ...Args>
        Ref<T> _CreateAsset(const String& name, Args&& ... args) {
            static_assert(std::is_base_of_v<IAsset, T>, "T must be an asset type!");
            String assetName = NormalizePath(name);
            Path assetPath(assetName);
            UUID assetID = UUIDUtils::Generate();
            auto ref = CreateRef<T>(std::forward<Args>(args)...);
            const auto* ptr = static_cast<const IAsset*>(ref.get());
            m_assetNames_IDs[assetName] = assetID;
            m_assetPointers_IDs[ptr] = assetID;
            m_loadedAssets[assetID] = WeakRef<IAsset>(ref);
            m_assetMeta[assetID] = {
                .id = assetID,
                .type = GetAssetType<T>(),
                .name = assetName,
                .displayName = assetPath.stem().string(),
                .path = assetPath,
                .ptr = ptr
            };
            return ref;
        }

        template<typename T>
        Ref<T> _LoadAsset(const String& name) {
            // Asset name, type, and ID
            String assetName = NormalizePath(name);
            AssetType assetType = GetAssetType<T>();
            UUID assetID;

            // Asset already loaded
            CX_MAP_FIND(m_assetNames_IDs, assetName, i_assetID) {
                assetID = i_assetID->second;
                CX_MAP_FIND(m_loadedAssets, assetID, i_asset) {
                    auto assetPtr = i_asset->second.lock();
                    return assetPtr->GetAssetType() == assetType ?
                           std::dynamic_pointer_cast<T>(assetPtr) :
                           Ref<T>();
                }
            }

            // Check file exists
            Path assetFile;
            if (!FindAssetFile(assetName, assetFile))
                return Ref<T>();

            // Check extension matches asset type
            if (!ExpectAssetType(assetFile, assetType))
                return Ref<T>();

            // Load asset from file
            T* asset = T::Create(assetFile.string());
            if (asset == nullptr)
                return Ref<T>();

            // Return ref
            CX_CORE_ASSERT(!assetID.is_nil(), "Invalid asset UUID!");
            const auto* ptr = static_cast<const IAsset*>(asset);
            Ref<T> ref(asset, AssetDeleter<T>(assetID));
            m_assetPointers_IDs[ptr] = assetID;
            m_loadedAssets[assetID] = WeakRef<IAsset>(ref);
            m_assetMeta[assetID].ptr = ptr;
            return ref;
        }

        Ref<IAsset> _LoadAsset(const UUID& id);

        template<typename T, typename ...Ext>
        void _RegisterAssetType(const Ext& ... extensions) {
            static_assert(std::is_base_of_v<IAsset, T>, "T must extend IAsset!");
            List<String> exts = { extensions... };
            auto assetType = GetAssetType<T>();
            m_registeredAssetTypes[assetType] = {
                .type = assetType,
                .loadAssetFn = reinterpret_cast<IAsset* (*)(const String&)>(
                    entt::overload<T*(const String&)>(&T::Create)
                )
            };
            for (const auto& ext: exts) {
                m_assetExtensions_Types[ext] = assetType;
            }
        }

        void _UnloadAll();

        void _AddSearchPath(const String& path);

        void _RemoveSearchPath(const String& path);

        void _SearchComponents(const String& query, const Set<AssetType>& excluded, List<AssetMeta>& outList);
        void _SearchAssets(AssetType assetType, const String& query, List<AssetMeta>& outList);

        UUID _GetAssetID(const IAsset* ptr);
        UUID _GetAssetID(const Ref<IAsset>& ref);

        String _GetAssetDisplayName(const IAsset* ptr);
        String _GetAssetDisplayName(const Ref<IAsset>& ref);

        const AssetMeta* _GetAssetMeta(const IAsset* ptr);
        const AssetMeta* _GetAssetMeta(const Ref<IAsset>& ref);

        void RemoveComponents();
        void RegisterComponents();

    private:
        Scope<efsw::FileWatcher> m_assetWatcher;

        List<AssetSearchPath> m_searchPaths{};

        Map<AssetType, RegisteredAssetType> m_registeredAssetTypes{};
        Map<String, AssetType> m_assetExtensions_Types{};

        Map<UUID, AssetMeta> m_assetMeta{};

        Map<UUID, WeakRef<IAsset>> m_loadedAssets{};
        Map<String, UUID> m_assetNames_IDs;
        Map<const IAsset*, UUID> m_assetPointers_IDs;

    };

}