#include "SandboxApp.h"

namespace Calyx {

    SandboxApp::SandboxApp() {

    }

    SandboxApp::~SandboxApp() {

    }

    Application* CreateApplication() {
        return new SandboxApp;
    }

}