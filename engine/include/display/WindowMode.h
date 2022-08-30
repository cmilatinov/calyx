#pragma once

//#include "reflect.h"

#define CX_DISPLAY_CENTER   -1
#define CX_DISPLAY_MAX_SIZE -2

namespace Calyx {

    /**
     * Dictates the way a display mode handles the mouse cursor.
     */
    enum class CursorMode {
        /**
        * Cursor can freely move in and out of the window without restrictions.
        */
        NORMAL,
        /**
        * Cursor is hidden when the window is in focus but can still exit window bounds.
        */
        HIDDEN,
        /**
        * Cursor is fully hidden and disabled (grabbed) while the window is in focus.
        */
        DISABLED
    };

    /**
     * Describes the way a window is shown by the operating system.
     */
    struct WindowMode {

        static const WindowMode FULLSCREEN;
        static const WindowMode BORDERLESS_FULLSCREEN;
        static const WindowMode DEFAULT;

        /**
         * The position of the window's top left corner in screen pixels.
         */
        int32 x, y;

        /**
         * The size of the window in pixels.
         */
        int32 width, height;

        /**
         * The cursor mode to use for the window.
         */
        CursorMode cursorMode;

        /**
         * If true, adds window borders and controls.
         */
        bool decorated;

        /**
         * If true, allows the window to be resized.
         */
        bool resizable;

        /**
         * If true, enables v-sync.
         */
        bool vsync;

        /**
         * If true, the window is always rendered on top of other windows.
         */
        bool alwaysOnTop;

        /**
         * If true, the window is shown in fullscreen mode (changes the monitor's display mode).
         */
        bool fullscreen;

    };

}