#pragma once

#include <string>
#include <filesystem>

namespace Calyx::PathUtils {

    std::filesystem::path Normalize(const std::filesystem::path& path);
    std::filesystem::path FirstExistingFile(const std::filesystem::path& path);
    std::filesystem::path GetUserHomeDirectory();

}