#include "EditorApp.h"

namespace Calyx {

    Application* CreateApplication() {
        return new Editor::EditorApp();
    }

}