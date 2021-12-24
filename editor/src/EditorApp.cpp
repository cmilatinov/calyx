#include "EditorApp.h"
#include "layers/EditorLayer.h"

namespace Calyx::Editor {

    EditorApp::EditorApp() {
        PushUnderlay(new EditorLayer());
    }

    EditorApp::~EditorApp() {

    }

}