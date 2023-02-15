#include "LauncherApp.h"
#include "layers/LauncherLayer.h"

namespace Calyx::Launcher {

    static const WindowMode s_launcherWindowMode{
        .x = CX_DISPLAY_CENTER,
        .y = CX_DISPLAY_CENTER,
        .width = 1024,
        .height = 576,
        .cursorMode = CursorMode::NORMAL,
        .decorated = true,
        .resizable = true,
        .vsync = true,
        .alwaysOnTop = false,
        .fullscreen = false
    };

    LauncherApp::LauncherApp() : Application(s_launcherWindowMode) {
        Window::GetMainWindow().SetTitle("Calyx Launcher");
        PushUnderlay(new LauncherLayer());
    }

}

namespace Calyx {

    Scope<Application> CreateApplication(int argc, char** argv) {
        return CreateScope<Launcher::LauncherApp>();
    }

}