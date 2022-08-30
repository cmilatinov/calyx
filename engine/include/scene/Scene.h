#pragma once

#include "assets/AssetRegistry.h"
#include "math/Transform.h"

namespace Calyx {

    class GameObject;
    class SceneRenderer;

    class Scene : public Asset<Scene> {

        friend class GameObject;
        friend class SceneRenderer;

    public:
        ~Scene() override;

        bool Load(const String& path) override;
        void Clear() override;

        GameObject* CreateGameObject(const String& name = "Game Object");
        void DeleteGameObject(GameObject* gameObject);

        const List<GameObject*>& GetRootGameObjects() const { return m_rootGameObjects; }

    private:
        entt::registry m_entityRegistry;
        Map<entt::entity, Scope<GameObject>> m_gameObjects;
        List<GameObject*> m_rootGameObjects;

    };

}