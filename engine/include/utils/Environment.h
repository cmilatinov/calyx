#pragma once

namespace Calyx::Utils {

    class Environment {

    public:
        static String Get(const String& name);
        static void Set(const String& name, const String& value);
        static void Add(const String& name, const String& value);

    };

}