#pragma once

#include "scene/Scene.h"
#include "ecs/GameObject.h"

namespace Calyx::Editor {

    class SceneHierarchyPanel {

    public:
        void Draw();

    private:
        Scene* m_scene = nullptr;

        void DrawGameObjectNode(GameObject* gameObject);

    };

}