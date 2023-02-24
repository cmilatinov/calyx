#include "FileUtils.h"

namespace Calyx::FileUtils {

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