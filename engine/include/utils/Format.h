#pragma once

namespace Calyx::Utils {

    class Format {

    public:
        static String ByteSize(uint64 bytes, int precision = 2);

        template<typename T>
        static inline String Number(T value, const String& suffix = "", int precision = 3) {
            static_assert(std::is_arithmetic_v<T>, "T must be an arithmetic type!");
            if constexpr (std::is_floating_point_v<T>) {
                char buffer[50];
                sprintf(buffer, ("%." + std::to_string(precision) + "f" + suffix).c_str(), value);
                return buffer;
            }
            return std::format("{}{}", value, suffix.c_str());
        }

        template<typename T>
        static String Percentage(T value, int precision = 1) {
            return Number(100 * value, " %%", precision);
        }

        template<typename T>
        static String Seconds(T value, int precision = 3) {
            return Number(value, " s", precision);
        }

        template<typename T>
        static String Milliseconds(T value, int precision = 3) {
            return Number(value, " ms", precision);
        }

        template<typename T>
        static String Nanoseconds(T value, int precision = 3) {
            return Number(value, " ns", precision);
        }

    };

}