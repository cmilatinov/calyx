#pragma once

namespace Calyx {

    class Utils {

    public:
        static String GetEnv(const String& name);
        static void SetEnv(const String& name, const String& value);
        static void AddEnv(const String& name, const String& value);

    };

}