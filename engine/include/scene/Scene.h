#pragma once

#include "assets/AssetRegistry.h"
#include "math/Transform.h"

namespace Calyx::Editor {
    class EditorLayer;
    class ViewportPanel;
    class InspectorPanel;
}

namespace Calyx {

    template<typename T>
    class Component;
    class GameObject;
    class SceneManager;
    class SceneRenderer;
    class SceneExporter;
    class SceneImporter;

    class CALYX_API Scene : public Asset<Scene> {

        template<typename T>
        friend
        class Component;
        friend class GameObject;
        friend class SceneManager;
        friend class SceneRenderer;
        friend class SceneExporter;
        friend class SceneImporter;
        friend class Editor::EditorLayer;
        friend class Editor::ViewportPanel;
        friend class Editor::InspectorPanel;

    public:
        Scene() = default;
        Scene(const Scene& other);
        ~Scene() override = default;

        GameObject*
        CreateGameObject(const String& name = "Game Object", GameObject* parent = nullptr, const UUID& id = UUID());
        void DeleteGameObject(GameObject* gameObject);

        GameObject* GetGameObject(const UUID& id) const;

        const Set<GameObject*>& GetRootGameObjects() const { return m_rootGameObjects; }

        bool Load(const String& path) override;

    private:
        bool m_firstFrame = true;
        entt::registry m_entityRegistry;
        Map<entt::entity, Scope<GameObject>> m_gameObjects;
        Map<UUID, entt::entity> m_gameObjectsById;
        Set<GameObject*> m_rootGameObjects;
        Set<entt::entity> m_objectsToDelete;

        bool Exists(entt::entity id);
        void DeleteGameObjectInternal(entt::entity id, bool eraseFromParent);
        void DeleteGameObjects();

        void Clear();

        void Copy(const Scene& other);

        void Update();

    };

}