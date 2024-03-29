#pragma once

#include "core/Platform.h"

#ifdef CX_PLATFORM_WINDOWS
#ifdef CX_BUILD_DLL
#ifdef CX_DLL_EXPORT
#define CALYX_API __declspec(dllexport)
#else
#define CALYX_API __declspec(dllimport)
#endif
#else
#define CALYX_API
#endif

#include <Windows.h>
#elif defined(CX_PLATFORM_LINUX)
#define CALYX_API
#else
#error "Calyx only supports Windows or Linux!"
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

#include "core/Assert.h"
#include "core/Log.h"