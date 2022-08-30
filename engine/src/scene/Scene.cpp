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
    }

    GameObject* Scene::CreateGameObject(const String& name) {
        entt::entity entityID = m_entityRegistry.create();
        m_gameObjects[entityID] = CreateScope<GameObject>(this, entityID, name);
        GameObject* gameObject = m_gameObjects[entityID].get();
        m_rootGameObjects.push_back(gameObject);
        // TODO: Add default components
        gameObject->AddComponent<TransformComponent>();
        return gameObject;
    }

    void Scene::DeleteGameObject(GameObject* gameObject) {

    }

}