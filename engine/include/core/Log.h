#pragma once

#include <spdlog/spdlog.h>

#include "core/Types.h"

namespace Calyx {

    class CALYX_API Log {
    CX_SINGLETON(Log);

    public:
        Log();

    public:
        CX_SINGLETON_EXPOSE_METHOD(_GetCoreLogger, Ref<spdlog::logger>& GetCoreLogger());
        CX_SINGLETON_EXPOSE_METHOD(_GetClientLogger, Ref<spdlog::logger>& GetClientLogger());

    private:
        Ref<spdlog::logger>& _GetCoreLogger() { return m_coreLogger; }
        Ref<spdlog::logger>& _GetClientLogger() { return m_clientLogger; }

    private:
        Ref<spdlog::logger> m_coreLogger;
        Ref<spdlog::logger> m_clientLogger;

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