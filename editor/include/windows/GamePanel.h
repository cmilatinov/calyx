#pragma once

#include "scene/Scene.h"

namespace Calyx::Editor {

    class GamePanel {

    public:
        GamePanel() = default;

        void Draw();

    private:
        Scene* m_scene;

    };

}