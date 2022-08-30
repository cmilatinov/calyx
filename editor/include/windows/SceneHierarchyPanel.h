#pragma once

#include <imgui.h>

#include "scene/Scene.h"
#include "ecs/GameObject.h"

namespace Calyx::Editor {

    class SceneHierarchyPanel {

    public:
        explicit SceneHierarchyPanel(Scene* scene);

        void Draw();

        GameObject* GetSelectedObject() { return m_selected; }

    private:
        Scene* m_scene;
        GameObject* m_selected = nullptr;

        void DrawGameObjectNode(GameObject* gameObject);

    };

}