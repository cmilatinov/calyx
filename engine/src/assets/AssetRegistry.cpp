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

    String AssetRegistry::NormalizePath(const String& path) {
        return Path(path).lexically_normal().string();
    }

    bool AssetRegistry::FindAssetType(const Path& file, AssetT& outType) {
        String ext = file.extension().string();
        auto extEntry = m_assetTypes.find(ext);
        if (extEntry == m_assetTypes.end())
            return false;
        outType = extEntry->second;
        return true;
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
            m_assetMetaMap.erase(asset);
        }
        m_searchPaths.erase(
            std::remove_if(
                m_searchPaths.begin(),
                m_searchPaths.end(),
                [&searchPath](const auto& sp) { return sp.path.string() == searchPath.path; }
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
            m_assetMetaMap[assetName] = {
                .type = assetType,
                .name = assetName,
                .path = FileSystem::absolute(file).lexically_normal().string()
            };
            searchPath.assets.push_back(assetName);
        }
    }

}