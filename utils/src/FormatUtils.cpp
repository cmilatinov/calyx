#include "FormatUtils.h"

namespace Calyx::FormatUtils {

    std::string ByteSize(uint64_t bytes, int precision) {
        static constexpr const uint64_t BASE = 10;
        static constexpr const char FILE_SIZE_UNITS[8][4] = {
            " B", " KB", " MB", " GB", " TB", " PB", " EB", " ZB"
        };

        uint64_t num = bytes;
        uint64_t power = 0;
        while (num >= CX_BIT(BASE)) {
            num >>= BASE;
            power++;
        }

        if (power > 0) {
            return Number((float)(bytes >> BASE * (power - 1)) / CX_BIT(BASE), FILE_SIZE_UNITS[power], precision);
        }

        return Number(bytes, FILE_SIZE_UNITS[power]);
    }

}