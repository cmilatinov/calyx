#include "scene/Scene.h"
#include "ecs/Component.h"
#include "ecs/components/TransformComponent.h"
#include "reflect/ClassRegistry.h"

namespace Calyx {

    Scene::Scene(const Scene& other) {
        Copy(other);
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

    GameObject* Scene::CreateGameObject(const String& name, GameObject* parent, const UUID& id) {
        // Create object
        auto uuid = id.is_nil() ? UUIDUtils::Generate() : id;
        entt::entity entityID = m_entityRegistry.create();
        m_gameObjectsById[uuid] = entityID;
        m_gameObjects[entityID] = Scope<GameObject>(new GameObject(this, uuid, entityID, name));

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
        return m_gameObjects.contains(id);
    }

    void Scene::DeleteGameObjectInternal(entt::entity id, bool eraseFromParent) {
        // Check exists
        if (!Exists(id)) {
            return;
        }

        // Delete children
        auto* gameObject = m_gameObjects[id].get();
        for (auto* child: gameObject->GetChildren()) {
            DeleteGameObjectInternal(child->m_entityID, false);
        }

        // Erase from children list of parent
        if (eraseFromParent && gameObject->m_parent != nullptr) {
            gameObject->m_parent->m_children.erase(gameObject);
        }

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

    GameObject* Scene::GetGameObject(const UUID& id) const {
        CX_MAP_FIND(m_gameObjectsById, id, i_entityID) {
            CX_MAP_FIND(m_gameObjects, i_entityID->second, i_gameObject) {
                return i_gameObject->second.get();
            }
        }
        return nullptr;
    }

    void Scene::Copy(const Scene& other) {
        for (const auto& [id, gameObject]: other.m_gameObjects) {
            CreateGameObject(gameObject->GetName(), nullptr, gameObject->GetID());
        }

        for (const auto& [componentID, set]: other.m_entityRegistry.storage()) {
            auto type = entt::resolve(componentID);
            for (const auto& [id, otherGameObject]: other.m_gameObjects) {
                if (set.contains(otherGameObject->m_entityID)) {
                    auto* gameObject = GetGameObject(otherGameObject->GetID());
                    gameObject->AddComponent(type, otherGameObject->GetComponent(type));
                }
            }
        }

        for (const auto& [id, gameObject]: m_gameObjects) {
            auto* otherGameObject = other.GetGameObject(gameObject->GetID());
            auto* parent = otherGameObject->GetParent();
            if (parent != nullptr) {
                gameObject->SetParent(GetGameObject(parent->GetID()));
            }
        }
    }

    void Scene::Update() {
        for (const auto& [component, functions]: ClassRegistry::GetComponentClasses()) {
            auto* storage = m_entityRegistry.storage(component.id());
            if (storage == nullptr)
                continue;

            bool doStart = m_firstFrame && static_cast<bool>(functions.start);
            bool doUpdate = static_cast<bool>(functions.update);
            for (const auto& entity: *storage) {
                auto ref = Reflect::Core::CreateOpaqueReference(component, storage->get(entity));
                if (doStart) {
                    functions.start.invoke(ref);
                }
                if (doUpdate) {
                    functions.update.invoke(ref);
                }
            }
        }

        if (m_firstFrame) {
            m_firstFrame = false;
        }
    }

}