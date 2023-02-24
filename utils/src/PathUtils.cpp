#include "PathUtils.h"

namespace Calyx::PathUtils {

    std::filesystem::path Normalize(const std::filesystem::path& path) {
        return std::filesystem::weakly_canonical(path);
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