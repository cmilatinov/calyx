#include "EditorApp.h"

namespace Calyx::Editor {

    EditorApp::EditorApp() {
        Inspector::Init();
        Window::GetMainWindow().SetTitle("Calyx Editor");
        PushUnderlay(new EditorLayer());
    }

}

namespace Calyx {

    Application* CreateApplication() {
        return new Editor::EditorApp();
    }

}