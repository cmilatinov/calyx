#pragma once

#include "Def.h"

namespace Calyx {

    class CALYX_API Application {

    public:
        Application();
        virtual ~Application();

        virtual void Run();

    };

    extern Application* CreateApplication();

}