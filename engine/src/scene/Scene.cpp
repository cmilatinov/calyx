#include "scene/Scene.h"
#include "ecs/GameObject.h"

namespace Calyx {

    Scene::~Scene() {
        AssetRegistry::UnloadAll();
    }

    bool Scene::Load(const String& path) {
        // TODO
        return false;
    }

    void Scene::Clear() {
        m_entityRegistry.clear();
        m_rootGameObjects.clear();
        m_gameObjects.clear();
        m_objectsToDelete.clear();
    }

    GameObject* Scene::CreateGameObject(const String& name, GameObject* parent) {
        // Create object
        entt::entity entityID = m_entityRegistry.create();
        m_gameObjects[entityID] = CreateScope<GameObject>(this, entityID, name);

        // TODO: Add default components
        GameObject* gameObject = m_gameObjects[entityID].get();
        gameObject->AddComponent<TransformComponent>();

        // Attach parent
        if (parent != nullptr) {
            CX_CORE_ASSERT(
                parent->m_scene == this,
                "Scene::CreateGameObject called with parent object from another scene!"
            );
            gameObject->SetParent(parent);
        } else {
            m_rootGameObjects.insert(gameObject);
        }

        return gameObject;
    }

    void Scene::DeleteGameObject(GameObject* gameObject) {
        CX_CORE_ASSERT(gameObject != nullptr, "Scene::DeleteGameObject called with null game object!");
        if (gameObject != nullptr) {
            m_objectsToDelete.insert(gameObject->m_entityID);
        }
    }

    bool Scene::Exists(entt::entity id) {
        return m_gameObjects.count(id) > 0;
    }

    void Scene::DeleteGameObjectInternal(entt::entity id, bool eraseFromParent) {
        // Check exists
        if (!Exists(id))
            return;

        // Delete children
        auto* gameObject = m_gameObjects[id].get();
        for (auto* child: gameObject->GetChildren()) {
            DeleteGameObjectInternal(child->m_entityID, false);
        }

        // Erase from children list of parent
        if (eraseFromParent && gameObject->m_parent != nullptr)
            gameObject->m_parent->m_children.erase(gameObject);

        // Destroy object
        m_entityRegistry.destroy(id);
        m_gameObjects.erase(id);
        m_rootGameObjects.erase(gameObject);
    }

    void Scene::DeleteGameObjects() {
        // Delete objects
        for (auto id: m_objectsToDelete) {
            DeleteGameObjectInternal(id, true);
        }

        // Clear list
        m_objectsToDelete.clear();
    }

}