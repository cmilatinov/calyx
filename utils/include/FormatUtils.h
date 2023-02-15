#pragma once

#include <string>
#include <fmt/format.h>

#include "Macros.h"

namespace Calyx::FormatUtils {

    std::string ByteSize(uint64_t bytes, int precision = 2);

    template<typename T>
    std::string Number(T value, const std::string& suffix = "", int precision = 3) {
        static_assert(std::is_arithmetic_v<T>, "T must be an arithmetic type!");
        if constexpr (std::is_floating_point_v<T>) {
            char buffer[50];
            snprintf(buffer, sizeof(buffer), ("%." + std::to_string(precision) + "f" + suffix).c_str(), value);
            return buffer;
        }
        return fmt::format("{}{}", value, suffix.c_str());
    }

    template<typename T>
    std::string Percentage(T value, int precision = 1) {
        return Number(100 * value, " %%", precision);
    }

    template<typename T>
    std::string Seconds(T value, int precision = 3) {
        return Number(value, " s", precision);
    }

    template<typename T>
    std::string Milliseconds(T value, int precision = 3) {
        return Number(value, " ms", precision);
    }

    template<typename T>
    std::string Nanoseconds(T value, int precision = 3) {
        return Number(value, " ns", precision);
    }

}