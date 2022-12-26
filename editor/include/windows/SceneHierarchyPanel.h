#pragma once

#include <imgui.h>

#include "inspector/SelectionManager.h"
#include "scene/Scene.h"
#include "ecs/GameObject.h"

namespace Calyx::Editor {

    class SceneHierarchyPanel {

    public:
        explicit SceneHierarchyPanel(Scene* scene);

        void Draw();

    private:
        Scene* m_scene;

        void DrawGameObjectNode(GameObject* gameObject);

    };

}