#pragma once

#include <cstring>
#include <sstream>
#include <string>
#include <vector>
#include <regex>
#include <algorithm>

namespace Calyx::StringUtils {

    template<typename T>
    T ToValue(const std::string& str) {
        std::stringstream ss(str);
        T value;
        ss >> std::boolalpha >> value;
        return value;
    }

    template<typename Container>
    std::string Join(const Container& items, const std::string& delimiter = "") {
        std::stringstream stream;
        int i = 0;
        for (const auto& item: items) {
            stream << item;
            if (i != items.size() - 1) {
                stream << delimiter;
            }
            i++;
        }
        return stream.str();
    }

    template<typename T>
    std::size_t Hash(const T& str) {
        return std::hash<T>()(str);
    }

    std::string ToLowercase(const std::string& str);
    std::vector<std::string> Split(const std::string& str, const std::string& regex);
    std::string Replace(const std::string& str, const std::string& regex, const std::string& replacement);
    int Compare(const std::string& first, const std::string& second);
    bool RegexMatch(const std::string& str, const std::string_view& regex);

    bool IsSearchMatch(const std::string& query, const std::string& searchTarget);

}