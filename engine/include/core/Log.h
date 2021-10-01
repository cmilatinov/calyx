#pragma once

#include <spdlog/spdlog.h>

#include "core/Types.h"

namespace Calyx {

    class CALYX_API Log {

    public:
        static void Init();

        static Ref<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
        static Ref<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

    private:
        static Ref<spdlog::logger> s_CoreLogger;
        static Ref<spdlog::logger> s_ClientLogger;

    };

}

// Core log macros
#define CX_CORE_TRACE(...)    ::Calyx::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define CX_CORE_INFO(...)     ::Calyx::Log::GetCoreLogger()->info(__VA_ARGS__)
#define CX_CORE_WARN(...)     ::Calyx::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define CX_CORE_ERROR(...)    ::Calyx::Log::GetCoreLogger()->error(__VA_ARGS__)
#define CX_CORE_CRITICAL(...) ::Calyx::Log::GetCoreLogger()->critical(__VA_ARGS__)

// Client log macros
#define CX_TRACE(...)         ::Calyx::Log::GetClientLogger()->trace(__VA_ARGS__)
#define CX_INFO(...)          ::Calyx::Log::GetClientLogger()->info(__VA_ARGS__)
#define CX_WARN(...)          ::Calyx::Log::GetClientLogger()->warn(__VA_ARGS__)
#define CX_ERROR(...)         ::Calyx::Log::GetClientLogger()->error(__VA_ARGS__)
#define CX_CRITICAL(...)      ::Calyx::Log::GetClientLogger()->critical(__VA_ARGS__)