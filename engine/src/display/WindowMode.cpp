#include "display/WindowMode.h"

/**
 * Defines some commonly used window modes.
 */
namespace Calyx {

    const WindowMode WindowMode::FULLSCREEN = {
        CX_DISPLAY_CENTER,
        CX_DISPLAY_CENTER,
        CX_DISPLAY_MAX_SIZE,
        CX_DISPLAY_MAX_SIZE,
        CursorMode::NORMAL,
        false,
        false,
        true,
        false,
        true
    };

    const WindowMode WindowMode::BORDERLESS_FULLSCREEN = {
        CX_DISPLAY_CENTER,
        CX_DISPLAY_CENTER,
        CX_DISPLAY_MAX_SIZE,
        CX_DISPLAY_MAX_SIZE,
        CursorMode::NORMAL,
        true,
        false,
        true,
        false,
        false
    };

    const WindowMode WindowMode::DEFAULT = {
        CX_DISPLAY_CENTER,
        CX_DISPLAY_CENTER,
        1920,
        1080,
        CursorMode::NORMAL,
        true,
        false,
        true,
        false,
        false
    };

}