#pragma once

#include "core/Platform.h"

#ifdef CX_PLATFORM_WINDOWS
    #ifdef CX_BUILD_DLL
        #define CALYX_API __declspec(dllexport)
    #else
        #define CALYX_API __declspec(dllimport)
    #endif
    #include <Windows.h>
#else
    #error "Calyx only supports Windows!"
#endif

#ifdef CX_DEBUG
    #if defined(CX_PLATFORM_WINDOWS)
		#define CX_DEBUGBREAK() __debugbreak()
	#elif defined(CX_PLATFORM_LINUX)
		#include <signal.h>
		#define CX_DEBUGBREAK() raise(SIGTRAP)
	#else
		#error "Platform doesn't support debugbreak yet!"
    #endif

	#define CX_ENABLE_ASSERTS
    #define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE
#else
    #define CX_DEBUGBREAK()
#endif

#define BIT(x) (1 << x)

#define CX_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }
#define CX_DISPATCH_EVENT(type, fn, event) DispatchEvent<type>(event, CX_BIND_EVENT_FN(fn))

#include "core/Assert.h"
#include "core/Log.h"