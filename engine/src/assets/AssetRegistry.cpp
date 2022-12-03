#include "assets/AssetRegistry.h"

namespace Calyx {

    CX_SINGLETON_INSTANCE(AssetRegistry);

    AssetRegistry::AssetRegistry()
        : m_rootAssetDirectory("./GameAssets"),
          m_assetWatcher(CreateScope<efsw::FileWatcher>()) {
        FileSystem::create_directories(m_rootAssetDirectory);
        String rootAssetDir = m_rootAssetDirectory.string();
        _AddSearchPath(rootAssetDir);
        m_assetWatcher->addWatch(rootAssetDir, this);
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
        m_searchPaths.insert(m_searchPaths.begin(), path);
    }

    void AssetRegistry::_RemoveSearchPath(const String& path) {
        m_searchPaths.erase(path);
    }

    String AssetRegistry::FindAssetFile(const String& asset) {
        for (const auto& path: m_searchPaths) {
            Path file = path / asset;
            if (FileSystem::exists(file) && FileSystem::is_regular_file(file))
                return file.string();
        }
        return asset;
    }

}