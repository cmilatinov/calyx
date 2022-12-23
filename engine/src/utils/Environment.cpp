#include "utils/Environment.h"

namespace Calyx::Utils {

    String Environment::Get(const String& name) {
#ifdef CX_PLATFORM_WINDOWS
        String value;
        DWORD strSize = GetEnvironmentVariableA(name.c_str(), value.data(), 0);
        value.resize(strSize);
        strSize = GetEnvironmentVariableA(name.c_str(), value.data(), strSize);
        value.resize(strSize);
        return value;
#else
        return "";
#endif
    }

    void Environment::Set(const String& name, const String& value) {
#ifdef CX_PLATFORM_WINDOWS
        SetEnvironmentVariableA(name.c_str(), value.c_str());
#endif
    }

    void Environment::Add(const Calyx::String& name, const Calyx::String& value) {
#ifdef CX_PLATFORM_WINDOWS
        String env = Get(name);
        env += ";" + value;
        Set(name, env);
#endif
    }

}