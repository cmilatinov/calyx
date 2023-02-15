#include "EditorApp.h"
#include "layers/EditorLayer.h"

namespace Calyx::Editor {

    EditorApp::EditorApp() {
        Inspector::Init();
        Window::GetMainWindow().SetTitle("Calyx Editor");
        PushUnderlay(new EditorLayer());
    }

}

namespace Calyx {

    Scope<Application> CreateApplication(int argc, char** argv) {
        return CreateScope<Editor::EditorApp>();
    }

}