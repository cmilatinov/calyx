#include "assets/AssetRegistry.h"

#include "ecs/Component.h"
#include "render/objects/Shader.h"
#include "render/objects/ShaderDefinition.h"
#include "render/objects/Texture2D.h"
#include "assets/Mesh.h"
#include "reflect/ClassRegistry.h"

namespace Calyx {

    CX_SINGLETON_INSTANCE(AssetRegistry);

    AssetRegistry::AssetRegistry()
        : m_assetWatcher(CreateScope<efsw::FileWatcher>()) {
        InitAssetTypes();
        RegisterComponents();
        m_assetWatcher->watch();
    }

    void AssetRegistry::handleFileAction(
        efsw::WatchID watchID,
        const std::string& dir,
        const std::string& filename,
        efsw::Action action,
        std::string oldFilename
    ) {
        Path file = Path(dir) / filename;
        switch (action) {
            case efsw::Actions::Add:
                CX_CORE_TRACE("DIR ({}) FILE ({}) has event ADD", dir, filename);
                break;
            case efsw::Actions::Delete:
                CX_CORE_TRACE("DIR ({}) FILE ({}) has event DELETE", dir, filename);
                break;
            case efsw::Actions::Modified:
                CX_CORE_TRACE("DIR ({}) FILE ({}) has event MODIFIED", dir, filename);
                for (const auto& searchPath: m_searchPaths) {
                    if (FileUtils::IsInDirectory(searchPath.path, file)) {
                        String name = FileSystem::relative(file, searchPath.path).string();
                        CX_MAP_FIND(m_assetNames_IDs, name, i_assetID) {
                            m_dirtyAssets.insert(i_assetID->second);
                        }
                        break;
                    }
                }
                break;
            case efsw::Actions::Moved:
                CX_CORE_TRACE("DIR ({}) FILE ({}) has event MOVED from ({})", dir, filename, oldFilename);
                break;
            default:
                break;
        }
    }

    void AssetRegistry::_ReloadAssets() {
        for (const auto& assetID: m_dirtyAssets) {
            CX_MAP_FIND(m_loadedAssets, assetID, i_asset) {
                CX_MAP_FIND(m_assetMeta, assetID, i_assetMeta) {
                    CX_CORE_TRACE("Reloading asset '{}' ...", i_assetMeta->second.name);
                    i_asset->second.lock()->Load(i_assetMeta->second.path.string());
                }
            }
        }
        m_dirtyAssets.clear();
    }

    Ref<IAsset> AssetRegistry::_LoadAsset(const UUID& id) {
        // Asset already loaded
        CX_MAP_FIND(m_loadedAssets, id, i_asset) {
            return i_asset->second.lock();
        }

        // Load asset by type
        CX_MAP_FIND(m_assetMeta, id, i_assetMeta) {
            CX_MAP_FIND(m_registeredAssetTypes, i_assetMeta->second.type, i_assetType) {
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
        Path normalizedPath = PathUtils::Normalize(path);
        if (!FileSystem::is_directory(normalizedPath)) return;
        m_searchPaths.push_back(
            {
                .path = normalizedPath,
                .watch = m_assetWatcher->addWatch(normalizedPath.string(), this),
                .assets = {},
            }
        );
        auto& searchPath = m_searchPaths.back();
        BuildAssetMeta(searchPath);
        for (const auto& [typeId, type]: m_registeredAssetTypes) {
            if (type.autoload) {
                LoadAllAssetsOfType(typeId, m_searchPaths.back());
            }
        }
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

    void AssetRegistry::_SearchComponents(
        const String& query,
        const Set<AssetType>& excluded,
        List<AssetMeta>& outList
    ) {
        for (const auto& [_, meta]: m_assetMeta) {
            if (meta.isComponent && !excluded.contains(meta.type) &&
                StringUtils::IsSearchMatch(query, meta.displayName))
                outList.push_back(meta);
        }
    }

    void AssetRegistry::_SearchAssets(AssetType assetType, const String& query, List<AssetMeta>& outList) {
        for (const auto& [_, meta]: m_assetMeta) {
            if (meta.type == assetType && StringUtils::IsSearchMatch(query, meta.displayName))
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
        if (ptr == nullptr)
            return "None";

        auto meta = _GetAssetMeta(ptr);
        if (meta == nullptr)
            return "";

        return meta->displayName;
    }

    String AssetRegistry::_GetAssetDisplayName(const Ref<IAsset>& ref) {
        return _GetAssetDisplayName(ref.get());
    }

    const AssetRegistry::AssetMeta* AssetRegistry::_GetAssetMeta(const IAsset* ptr) {
        CX_MAP_FIND(m_assetPointers_IDs, ptr, i_assetID) {
            CX_MAP_FIND(m_assetMeta, i_assetID->second, i_assetMeta) {
                return &i_assetMeta->second;
            }
        }
        return nullptr;
    }

    const AssetRegistry::AssetMeta* AssetRegistry::_GetAssetMeta(const Ref<IAsset>& ref) {
        return _GetAssetMeta(ref.get());
    }

    String AssetRegistry::NormalizePath(const String& path) {
        return Path(path).lexically_normal().string();
    }

    bool AssetRegistry::FindAssetType(const Path& file, AssetType& outType) {
        String ext = file.extension().string();
        CX_MAP_FIND(m_assetExtensions_Types, ext, i_assetType) {
            outType = i_assetType->second;
            return true;
        }
        return false;
    }

    bool AssetRegistry::ExpectAssetType(const Path& file, AssetType expectedType) {
        AssetType actualType;
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
        _RegisterAssetType<Mesh>(false, ".obj", ".fbx", ".3ds", ".blend", ".ply");
        _RegisterAssetType<Shader>(false, ".glsl");
        _RegisterAssetType<ShaderDefinition>(true, ".glsldef");
        _RegisterAssetType<Texture2D>(false, ".png", ".jpg", ".bmp");
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
            AssetType assetType;
            if (!FindAssetType(file, assetType))
                continue;

            UUID assetID;
            json meta;
            if (LoadMetaFile(file.path(), meta)) {
                auto id = meta["id"].get<String>();
                assetID = UUID::from_string(id).value_or(UUIDUtils::Generate());
                m_assetNames_IDs[assetName] = assetID;
                m_assetMeta[assetID] = {
                    .id = assetID,
                    .type = assetType,
                    .name = assetName,
                    .displayName = meta["display_name"].get<String>(),
                    .path = FileSystem::absolute(file)
                };
            } else {
                assetID = UUIDUtils::Generate();
                m_assetNames_IDs[assetName] = assetID;
                m_assetMeta[assetID] = {
                    .id = assetID,
                    .type = assetType,
                    .name = assetName,
                    .displayName = relativeFilePath.stem().string(),
                    .path = FileSystem::absolute(file)
                };
                WriteMetaFile(file.path(), m_assetMeta[assetID]);
            }

            searchPath.assets.push_back(assetID);
        }
    }

    void AssetRegistry::LoadAllAssetsOfType(AssetType type, AssetSearchPath& searchPath) {
        CX_MAP_FIND(m_registeredAssetTypes, type, i_assetType) {
            List<String> patterns;
            for (const auto& [ext, assetType]: m_assetExtensions_Types) {
                if (assetType == type) {
                    patterns.push_back(fmt::format("**{}", ext.c_str()));
                }
            }
            auto files = FileUtils::GlobRecursive(searchPath.path, patterns);
            for (const auto& file: files) {
                String assetName = FileSystem::relative(file, searchPath.path);
                CX_MAP_FIND(m_assetNames_IDs, assetName, i_assetID) {
                    auto ref = LoadAsset(i_assetID->second);
                    if (ref) {
                        i_assetType->second.loadedAssets.push_back(ref);
                    }
                }
            }
        }
    }

    Path AssetRegistry::GetMetaFile(const Path& assetFile) {
        return assetFile.parent_path() / (assetFile.stem().string() + ".meta");
    }

    bool AssetRegistry::LoadMetaFile(const Path& assetFile, json& meta) {
        Path metaFile = GetMetaFile(assetFile);
        if (!FileSystem::exists(metaFile) || !FileSystem::is_regular_file(metaFile))
            return false;
        std::ifstream stream(metaFile);
        meta = json::parse(stream);
        return true;
    }

    void AssetRegistry::WriteMetaFile(const Path& assetFile, const AssetMeta& meta) {
        Path metaFile = GetMetaFile(assetFile);
        std::ofstream stream(metaFile);
        if (!stream.is_open()) return;
        json metaJSON = {
            { "id", uuids::to_string(meta.id) },
            { "display_name", meta.displayName }
        };
        stream << std::setw(4) << metaJSON;
    }

    void AssetRegistry::RemoveComponents() {
        List<UUID> components;
        for (const auto& [id, meta]: m_assetMeta) {
            if (meta.isComponent) {
                components.push_back(id);
            }
        }
        for (const auto& id: components) {
            m_assetNames_IDs.erase(m_assetMeta[id].name);
            m_assetMeta.erase(id);
        }
    }

    void AssetRegistry::RegisterComponents() {
        RemoveComponents();
        for (const auto& [_, info]: ClassRegistry::GetComponentClasses()) {
            auto temp = info.type.construct();
            if (!temp)
                continue;

            auto* tempComponent = temp.try_cast<IComponent>();
            if (tempComponent == nullptr)
                continue;

            UUID assetID = UUIDUtils::Generate();
            auto name = tempComponent->GetClassName();
            m_assetNames_IDs[name] = assetID;
            m_assetMeta[assetID] = {
                .id = assetID,
                .type = tempComponent->GetTypeId(),
                .name = name,
                .displayName = tempComponent->GetDisplayName(),
                .isComponent = true
            };
        }
    }

}