#pragma once

#include "assets/AssetRegistry.h"
#include "math/Transform.h"

namespace Calyx::Editor {
    class EditorLayer;
}

namespace Calyx {

    class GameObject;
    class SceneRenderer;

    class CALYX_API Scene : public Asset<Scene> {

        friend class GameObject;
        friend class SceneRenderer;
        friend class Editor::EditorLayer;

    public:
        ~Scene() override;

        GameObject* CreateGameObject(const String& name = "Game Object", GameObject* parent = nullptr);
        void DeleteGameObject(GameObject* gameObject);

        const Set<GameObject*>& GetRootGameObjects() const { return m_rootGameObjects; }

    private:
        entt::registry m_entityRegistry;
        Map<entt::entity, Scope<GameObject>> m_gameObjects;
        Set<GameObject*> m_rootGameObjects;
        Set<entt::entity> m_objectsToDelete;

        bool Exists(entt::entity id);
        void DeleteGameObjectInternal(entt::entity id, bool eraseFromParent);
        void DeleteGameObjects();

        bool Load(const String& path);
        void Clear();

    };

}