#include "display/WindowMode.h"

/**
 * Defines some commonly used window modes.
 */
namespace Calyx {

    const WindowMode WindowMode::FULLSCREEN = {
        .x = CX_DISPLAY_CENTER,
        .y = CX_DISPLAY_CENTER,
        .width = CX_DISPLAY_MAX_SIZE,
        .height = CX_DISPLAY_MAX_SIZE,
        .cursorMode = CursorMode::NORMAL,
        .decorated = false,
        .resizable = false,
        .vsync = true,
        .alwaysOnTop = false,
        .fullscreen = true
    };

    const WindowMode WindowMode::BORDERLESS_FULLSCREEN = {
        .x = CX_DISPLAY_CENTER,
        .y = CX_DISPLAY_CENTER,
        .width = CX_DISPLAY_MAX_SIZE,
        .height = CX_DISPLAY_MAX_SIZE,
        .cursorMode = CursorMode::NORMAL,
        .decorated = true,
        .resizable = false,
        .vsync = true,
        .alwaysOnTop = false,
        .fullscreen = false
    };

    const WindowMode WindowMode::DEFAULT = {
        .x = CX_DISPLAY_CENTER,
        .y = CX_DISPLAY_CENTER,
        .width = 1920,
        .height = 1080,
        .cursorMode = CursorMode::NORMAL,
        .decorated = true,
        .resizable = true,
        .vsync = true,
        .alwaysOnTop = false,
        .fullscreen = false
    };

}