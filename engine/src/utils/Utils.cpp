#include "utils/Utils.h"

namespace Calyx {

    String Utils::GetEnv(const String& name) {
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

    void Utils::SetEnv(const String& name, const String& value) {
#ifdef CX_PLATFORM_WINDOWS
        SetEnvironmentVariableA(name.c_str(), value.c_str());
#endif
    }

    void Utils::AddEnv(const Calyx::String& name, const Calyx::String& value) {
#ifdef CX_PLATFORM_WINDOWS
        String env = GetEnv(name);
        env += ";" + value;
        SetEnv(name, env);
#endif
    }

}