#include "EditorApp.h"
#include "layers/EditorLayer.h"

namespace Calyx::Editor {

    EditorApp::EditorApp() {
        PushUnderlay(new EditorLayer());
    }

    EditorApp::~EditorApp() {

    }

}

namespace Calyx {

    Application* CreateApplication() {
        return new Editor::EditorApp();
    }

}