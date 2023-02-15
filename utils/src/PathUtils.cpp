#include "PathUtils.h"

namespace Calyx::PathUtils {

    std::filesystem::path RemoveTrailingSeparator(const std::filesystem::path& path) {
        if (path.empty())
            return path;
        auto size = std::distance(path.begin(), path.end());
        auto it = path.begin();
        std::advance(it, size - 1);
        if (!(*it).empty())
            return path;
        return path.parent_path();
    }

    std::filesystem::path Normalize(const std::filesystem::path& path) {
        if (path.empty())
            return path;
        return PathUtils::RemoveTrailingSeparator(std::filesystem::absolute(path));
    }

    std::filesystem::path FirstExistingFile(const std::filesystem::path& path) {
        std::filesystem::path result = Normalize(path);
        while (!std::filesystem::exists(result) && !result.empty()) {
            result = result.parent_path();
        }
        return result;
    }

    std::filesystem::path GetUserHomeDirectory() {
#if defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__) || defined(__MACH__)
        return Normalize(std::getenv("HOME"));
#elif defined(_WIN32) || defined(_WIN64)
        return Normalize(std::getenv("USERPROFILE"));
#else
        return {};
#endif
    }

}