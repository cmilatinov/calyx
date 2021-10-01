#pragma once

#include "core/Types.h"

namespace Calyx {

    using MouseCode = uint16;

    enum : MouseCode {
        MOUSE_BUTTON_0 = 0,
        MOUSE_BUTTON_1 = 1,
        MOUSE_BUTTON_2 = 2,
        MOUSE_BUTTON_3 = 3,
        MOUSE_BUTTON_4 = 4,
        MOUSE_BUTTON_5 = 5,
        MOUSE_BUTTON_6 = 6,
        MOUSE_BUTTON_7 = 7,

        MOUSE_BUTTON_LAST = MOUSE_BUTTON_7,
        MOUSE_BUTTON_LEFT = MOUSE_BUTTON_0,
        MOUSE_BUTTON_RIGHT = MOUSE_BUTTON_1,
        MOUSE_BUTTON_MIDDLE = MOUSE_BUTTON_2
    };

}