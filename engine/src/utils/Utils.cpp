#include "utils/Utils.h"

namespace Calyx::Utils {

    String ToLowercase(const String& str) {
        String result(str);
        std::transform(
            result.begin(),
            result.end(),
            result.begin(),
            [](auto c) { return std::tolower(c); }
        );
        return result;
    }

    List<String> Split(const String& str, const String& regex) {
        std::regex rgx(regex);
        std::sregex_token_iterator it(str.begin(), str.end(), rgx, -1);
        List<String> result;
        std::transform(
            std::sregex_token_iterator(str.begin(), str.end(), rgx, -1),
            std::sregex_token_iterator(),
            std::back_inserter(result),
            [](auto str) { return str; }
        );
        return result;
    }

    bool IsSearchMatch(const String& query, const String& target) {
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
                return targetLower.find(word) != String::npos;
            }
        );
    }

    UUID GenerateUUID() {
        static uuids::uuid_system_generator gen{};
        return gen();
    }

}