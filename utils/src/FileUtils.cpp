#include "FileUtils.h"

namespace Calyx::FileUtils {

    bool LongestCommonPrefix(
        const std::filesystem::path& first,
        const std::filesystem::path& second,
        std::filesystem::path& outPrefix
    ) {
        outPrefix = std::filesystem::path();
        auto it1 = first.begin();
        auto it2 = second.begin();
        while (it1 != first.end() && it2 != second.end() && *it1 == *it2) {
            outPrefix /= *it1;
            ++it1;
            ++it2;
        }
        return !outPrefix.empty();
    }

    bool IsInDirectory(const std::filesystem::path& directory, const std::filesystem::path& file) {
        return !std::filesystem::relative(file, directory).string().starts_with("..");
    }

    std::string ReadEntireFile(const std::string& path) {
        std::ifstream file(path);
        if (!file.is_open()) return "";
        std::stringstream ss;
        ss << file.rdbuf();
        return ss.str();
    }

    static std::vector<std::regex> MapGlobPatterns(const std::vector<std::string>& patterns) {
        std::vector<std::regex> regexPatterns;
        std::transform(
            patterns.begin(), patterns.end(),
            std::back_inserter(regexPatterns),
            [](const std::string& pattern) {
                std::string result = StringUtils::Replace(pattern + "$", "\\*\\*", ".*");
                result = StringUtils::Replace(result, "\\*", "[^/]*");
                return std::regex(result);
            }
        );
        return regexPatterns;
    }

    std::vector<std::string> Glob(
        const std::filesystem::path& directory,
        const std::vector<std::string>& patterns
    ) {
        std::vector<std::regex> regexPatterns = MapGlobPatterns(patterns);
        std::vector<std::string> result;
        GlobCollect(directory, directory, regexPatterns, result);
        return result;
    }

    std::vector<std::string> GlobRecursive(
        const std::filesystem::path& directory,
        const std::vector<std::string>& patterns
    ) {
        std::vector<std::regex> regexPatterns = MapGlobPatterns(patterns);
        std::vector<std::string> result;
        GlobRecursiveCollect(directory, directory, regexPatterns, result);
        return result;
    }

}