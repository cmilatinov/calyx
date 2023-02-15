#include "StringUtils.h"

namespace Calyx::StringUtils {

    std::string ToLowercase(const std::string& str) {
        std::string result(str);
        std::transform(
            result.begin(),
            result.end(),
            result.begin(),
            [](auto c) { return std::tolower(c); }
        );
        return result;
    }

    std::vector<std::string> Split(const std::string& str, const std::string& regex) {
        std::regex rgx(regex);
        std::sregex_token_iterator it(str.begin(), str.end(), rgx, -1);
        std::vector<std::string> result;
        std::transform(
            std::sregex_token_iterator(str.begin(), str.end(), rgx, -1),
            std::sregex_token_iterator(),
            std::back_inserter(result),
            [](auto str) { return str; }
        );
        return result;
    }

    std::string Replace(const std::string& str, const std::string& regex, const std::string& replacement) {
        std::regex rgx(regex);
        return std::regex_replace(str, rgx, replacement);
    }

    int Compare(const std::string& first, const std::string& second) {
        return std::strcmp(first.c_str(), second.c_str());
    }

    bool IsSearchMatch(const std::string& query, const std::string& target) {
        auto words = Split(query, "\\s+");
        std::transform(
            words.begin(),
            words.end(),
            words.begin(),
            [](const auto& word) { return ToLowercase(word); }
        );

        auto targetLower = ToLowercase(target);
        return std::all_of(
            words.begin(),
            words.end(),
            [&targetLower](const auto& word) {
                return targetLower.find(word) != std::string::npos;
            }
        );
    }

}