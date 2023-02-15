#include "ecs/GameObject.h"
#include "ecs/components/TransformComponent.h"

namespace Calyx {

    GameObject::GameObject(Scene* scene, UUID id, entt::entity entityID, const String& name)
        : m_scene(scene),
          m_id(id),
          m_entityID(entityID),
          m_name(name) {}

    bool GameObject::HasComponent(const entt::meta_type& type) {
        auto storage = m_scene->m_entityRegistry.storage(type.id());
        if (storage == nullptr)
            return false;
        return storage->get(m_entityID) != nullptr;
    }

    void GameObject::AddComponent(const entt::meta_type& type) {
        auto instance = type.construct();
        if (!instance)
            return;

        auto instancePtr = instance.try_cast<IComponent>();
        if (instancePtr == nullptr)
            return;

        instancePtr->Emplace(m_entityID, m_scene->m_entityRegistry);
    }

    void GameObject::RemoveComponent(const entt::meta_type& type) {
        auto instance = type.construct();
        if (!instance)
            return;

        auto instancePtr = instance.try_cast<IComponent>();
        if (instancePtr == nullptr)
            return;

        instancePtr->Erase(m_entityID, m_scene->m_entityRegistry);
    }

    void GameObject::AddChild(GameObject* child) {
        CX_CORE_ASSERT(child != nullptr, "GameObject::AddChild called will null child!");
        CX_CORE_ASSERT(m_scene == child->m_scene,
                       "GameObject::AddChild called with GameObject belonging to another scene!");

        // Swap previous parent
        if (child->m_parent != nullptr)
            child->m_parent->m_children.erase(child);

        m_children.insert(child);
        child->m_parent = this;
        child->GetTransform().SetParent(&GetTransform());
        m_scene->m_rootGameObjects.erase(child);
    }

    void GameObject::SetParent(GameObject* parent) {
        CX_CORE_ASSERT(parent != nullptr, "GameObject::SetParent called will null parent!");
        CX_CORE_ASSERT(m_scene == parent->m_scene,
                       "GameObject::SetParent called with GameObject belonging to another scene!");

        // Swap previous parent
        if (m_parent != nullptr)
            m_parent->m_children.erase(this);

        parent->m_children.insert(this);
        m_parent = parent;
        GetTransform().SetParent(&parent->GetTransform());
        m_scene->m_rootGameObjects.erase(this);
    }

    void GameObject::DetachFromParent() {
        if (m_parent == nullptr)
            return;
        m_parent->m_children.erase(this);
        m_parent = nullptr;
        GetTransform().SetParent(nullptr);
        m_scene->m_rootGameObjects.insert(this);
    }

    Transform& GameObject::GetTransform() {
        auto* tc = GetComponent<TransformComponent>();
        CX_CORE_ASSERT(tc != nullptr, "GameObject has no TransformComponent attached!");
        return tc->GetTransform();
    }

    bool operator==(const GameObject& left, const GameObject& right) {
        return left.m_entityID == right.m_entityID;
    }

    bool operator!=(const GameObject& left, const GameObject& right) {
        return !(left == right);
    }

}