#include "ecs/GameObject.h"

namespace Calyx {

    GameObject::GameObject(Scene* scene, entt::entity entityID, const String& name)
        : m_scene(scene),
          m_entityID(entityID),
          m_name(name) {}

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


}