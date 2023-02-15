#include "SandboxApp.h"

namespace Calyx {

    Scope<Application> CreateApplication(int argc, char** argv) {
        return CreateScope<SandboxApp>();
    }

}