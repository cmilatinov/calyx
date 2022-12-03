#include "utils/Format.h"

namespace Calyx {

    String Format::ByteSize(uint64 bytes, int precision) {
        static constexpr const uint64 BASE = 10;
        static constexpr const char FILE_SIZE_UNITS[8][4] = {
            " B", " KB", " MB", " GB", " TB", " PB", " EB", " ZB"
        };

        uint64 num = bytes;
        uint64 power = 0;
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