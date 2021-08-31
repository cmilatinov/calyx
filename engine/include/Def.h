#pragma once

#ifdef CX_PLATFORM_WINDOWS
    #ifdef CX_BUILD_DLL
        #define CALYX_API __declspec(dllexport)
    #else
        #define CALYX_API __declspec(dllimport)
    #endif
#else
    #error Calyx only supports Windows!
#endif