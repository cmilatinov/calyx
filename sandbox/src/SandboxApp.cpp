#include "SandboxApp.h"
#include "layers/TestLayer.h"

namespace Calyx {

    SandboxApp::SandboxApp() {
        PushUnderlay(new TestLayer());
    }

    SandboxApp::~SandboxApp() {

    }

    Application* CreateApplication() {
        return new SandboxApp;
    }

}