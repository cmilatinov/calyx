#include "assets/AssetRegistry.h"

#include "render/objects/Shader.h"
#include "render/objects/Texture2D.h"
#include "assets/Mesh.h"

namespace Calyx {

    CX_SINGLETON_INSTANCE(AssetRegistry);

    AssetRegistry::AssetRegistry()
        : m_assetWatcher(CreateScope<efsw::FileWatcher>()) {
        InitAssetTypes();
        _AddSearchPath("assets");
        m_assetWatcher->watch();
    }

    void AssetRegistry::handleFileAction(
        efsw::WatchID watchID,
        const std::string& dir,
        const std::string& filename,
        efsw::Action action,
        std::string oldFilename
    ) {
        switch (action) {
            case efsw::Actions::Add:
                CX_CORE_TRACE("DIR ({}) FILE ({}) has event ADD", dir, filename);
                break;
            case efsw::Actions::Delete:
                CX_CORE_TRACE("DIR ({}) FILE ({}) has event DELETE", dir, filename);
                break;
            case efsw::Actions::Modified:
                CX_CORE_TRACE("DIR ({}) FILE ({}) has event MODIFIED", dir, filename);
                break;
            case efsw::Actions::Moved:
                CX_CORE_TRACE("DIR ({}) FILE ({}) has event MOVED from ({})", dir, filename, oldFilename);
                break;
            default:
                break;
        }
    }

    Ref<IAsset> AssetRegistry::_LoadAsset(const UUID& id) {
        // Asset already loaded
        CX_MAP_FIND(m_loadedAssets, id, i_asset) {
            return i_asset->second.lock();
        }

        // Load asset by type
        CX_MAP_FIND(m_assetMeta, id, i_assetMeta) {
            CX_MAP_FIND(m_assetTypes, i_assetMeta->second.type, i_assetType) {
                auto* asset = i_assetType->second.loadAssetFn(i_assetMeta->second.path.string());
                if (asset == nullptr)
                    return Ref<IAsset>();

                auto ref = Ref<IAsset>(asset, AssetDeleter<IAsset>(id));
                m_assetPointers_IDs[asset] = id;
                m_loadedAssets[id] = WeakRef<IAsset>(ref);
                i_assetMeta->second.ptr = asset;
                return ref;
            }
        }

        // No such asset
        return Ref<IAsset>();
    }

    void AssetRegistry::_UnloadAll() {
        m_loadedAssets.clear();
    }

    void AssetRegistry::_AddSearchPath(const String& path) {
        Path normalizedPath = NormalizePath(FileSystem::absolute(path).string());
        m_searchPaths.push_back(
            {
                .path = normalizedPath,
                .watch = m_assetWatcher->addWatch(normalizedPath.string(), this),
                .assets = {},
            }
        );
        BuildAssetMeta(m_searchPaths.back());
    }

    void AssetRegistry::_RemoveSearchPath(const String& path) {
        Path normalizedPath = NormalizePath(FileSystem::absolute(path).string());
        auto it = std::find_if(
            m_searchPaths.begin(),
            m_searchPaths.end(),
            [&normalizedPath](const auto& searchPath) {
                return searchPath.path == normalizedPath;
            }
        );
        if (it != m_searchPaths.end()) {
            m_assetWatcher->removeWatch(it->watch);
            ClearAssetMeta(*it);
            m_searchPaths.erase(it);
        }
    }

    void AssetRegistry::_SearchAssets(AssetT assetType, const String& query, List<AssetMeta>& outList) {
        for (const auto& [_, meta]: m_assetMeta) {
            if (meta.type == assetType && Utils::IsSearchMatch(query, meta.displayName))
                outList.push_back(meta);
        }
    }

    UUID AssetRegistry::_GetAssetID(const IAsset* ptr) {
        CX_MAP_FIND(m_assetPointers_IDs, ptr, i_assetID) {
            return i_assetID->second;
        }
        return UUID();
    }

    UUID AssetRegistry::_GetAssetID(const Ref<IAsset>& ref) {
        return _GetAssetID(ref.get());
    }

    String AssetRegistry::_GetAssetDisplayName(const IAsset* ptr) {
        if (ptr == nullptr) return "None";
        CX_MAP_FIND(m_assetPointers_IDs, ptr, i_assetID) {
            CX_MAP_FIND(m_assetMeta, i_assetID->second, i_assetMeta) {
                return i_assetMeta->second.displayName;
            }
        }
        return "";
    }

    String AssetRegistry::_GetAssetDisplayName(const Ref<IAsset>& ref) {
        return _GetAssetDisplayName(ref.get());
    }

    AssetRegistry::AssetMeta AssetRegistry::_GetAssetMeta(const IAsset* ptr) {
        CX_MAP_FIND(m_assetPointers_IDs, ptr, i_assetID) {
            CX_MAP_FIND(m_assetMeta, i_assetID->second, i_assetMeta) {
                return i_assetMeta->second;
            }
        }
        return {};
    }

    AssetRegistry::AssetMeta AssetRegistry::_GetAssetMeta(const Ref<IAsset>& ref) {
        return _GetAssetMeta(ref.get());
    }

    String AssetRegistry::NormalizePath(const String& path) {
        return Path(path).lexically_normal().string();
    }

    bool AssetRegistry::FindAssetType(const Path& file, AssetT& outType) {
        String ext = file.extension().string();
        CX_MAP_FIND(m_assetExtensions_Types, ext, i_assetType) {
            outType = i_assetType->second;
            return true;
        }
        return false;
    }

    bool AssetRegistry::ExpectAssetType(const Path& file, AssetT expectedType) {
        AssetT actualType;
        if (!FindAssetType(file, actualType))
            return false;
        return expectedType == actualType;
    }

    bool AssetRegistry::FindAssetFile(const String& asset, Path& outFilePath) {
        for (const auto& searchPath: m_searchPaths) {
            Path file = (searchPath.path / asset).lexically_normal();
            if (FileSystem::exists(file) && FileSystem::is_regular_file(file)) {
                outFilePath = file;
                return true;
            }
        }
        return false;
    }

    void AssetRegistry::InitAssetTypes() {
        _RegisterAssetType<Mesh>(".obj", ".fbx", ".3ds", ".blend", ".ply");
        _RegisterAssetType<Shader>(".glsl");
        _RegisterAssetType<Texture2D>(".png", ".jpg", ".bmp");
    }

    void AssetRegistry::ClearAssetMeta(AssetSearchPath& searchPath) {
        for (const auto& asset: searchPath.assets) {
            m_assetMeta.erase(asset);
        }
        m_searchPaths.erase(
            std::remove_if(
                m_searchPaths.begin(),
                m_searchPaths.end(),
                [&searchPath](const auto& sp) {
                    return FileSystem::equivalent(sp.path, searchPath.path);
                }
            ),
            m_searchPaths.end()
        );
    }

    void AssetRegistry::BuildAssetMeta(AssetSearchPath& searchPath) {
        for (const auto& file: FileSystem::recursive_directory_iterator(searchPath.path)) {
            if (!FileSystem::is_regular_file(file))
                continue;

            auto relativeFilePath = FileSystem::relative(file, searchPath.path);
            auto assetName = NormalizePath(relativeFilePath.string());
            AssetT assetType;
            if (!FindAssetType(file, assetType))
                continue;

            UUID assetID = Utils::GenerateUUID();
            m_assetNames_IDs[assetName] = assetID;
            m_assetMeta[assetID] = {
                .id = assetID,
                .type = assetType,
                .name = assetName,
                .displayName = relativeFilePath.stem().string(),
                .path = FileSystem::absolute(file).lexically_normal().string()
            };
            searchPath.assets.push_back(assetID);
        }
    }

}