#pragma once

namespace reflect::tooling::utils {

    std::string AccessToString(AccessSpecifier access);

    bool ReplaceInString(std::string& str, const std::string& occurrence, const std::string& replacement);

    template<typename T>
    std::string Join(
        const std::string& separator,
        const std::vector<T>& elements,
        const std::function<std::string(const T&)>& transformFn = [](const T& elem) {
            std::ostringstream oss;
            oss << elem;
            return oss.str();
        }
    ) {
        std::ostringstream oss;
        size_t last = elements.size() - 1;
        std::for_each(elements.begin(), elements.end(),
                      [&separator, &transformFn, &oss, last, i = 0](const T& elem) mutable {
                          oss << transformFn(elem);
                          if (i != last)
                              oss << separator;
                          i++;
                      });
        return oss.str();
    }


    static const std::string STORAGE_TYPE_STATIC = "StorageType::STATIC"; // NOLINT(cert-err58-cpp)
    static const std::string STORAGE_TYPE_INSTANCE = "StorageType::INSTANCE"; // NOLINT(cert-err58-cpp)

}