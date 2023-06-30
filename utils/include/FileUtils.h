#pragma once

#include <string>
#include <regex>
#include <vector>
#include <filesystem>
#include <sstream>
#include <fstream>

#include "StringUtils.h"

namespace Calyx::FileUtils {

    bool LongestCommonPrefix(
        const std::filesystem::path& first,
        const std::filesystem::path& second,
        std::filesystem::path& outPrefix
    );

    bool IsInDirectory(const std::filesystem::path& directory, const std::filesystem::path& file);

    std::string ReadEntireFile(const std::string& path);

    template<typename OutContainer>
    void GlobCollect(
        const std::filesystem::path& root,
        const std::filesystem::path& directory,
        const std::vector<std::regex>& patterns,
        OutContainer& out,
        bool recurse = false
    ) {
        if (!std::filesystem::is_directory(directory)) return;
        for (const auto& file: std::filesystem::directory_iterator(directory)) {
            auto filePath = std::filesystem::relative(file.path(), root);
            auto fileStatus = file.status();
            if (std::filesystem::is_regular_file(fileStatus) &&
                std::any_of(
                    patterns.begin(), patterns.end(),
                    [&filePath](const auto& regex) { return std::regex_search(filePath.string(), regex); }
                )) {
                out.push_back(file.path());
            } else if (std::filesystem::is_directory(fileStatus) && recurse) {
                GlobCollect(root, file.path(), patterns, out, true);
            }
        }
    }

    template<typename OutContainer>
    void GlobRecursiveCollect(
        const std::filesystem::path& root,
        const std::filesystem::path& directory,
        const std::vector<std::regex>& patterns,
        OutContainer& out
    ) {
        GlobCollect(root, directory, patterns, out, true);
    }

    std::vector<std::string> Glob(
        const std::filesystem::path& directory,
        const std::vector<std::string>& patterns
    );

    std::vector<std::string> GlobRecursive(
        const std::filesystem::path& directory,
        const std::vector<std::string>& patterns
    );

}