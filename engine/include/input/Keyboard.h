#pragma once

#include <GLFW/glfw3.h>

#include "core/Types.h"
#include "events/KeyCodes.h"

namespace Calyx {

    class Keyboard {

    public:
        static bool IsKeyDown(KeyCode key) {
            return glfwGetKey()
        }


    };

}