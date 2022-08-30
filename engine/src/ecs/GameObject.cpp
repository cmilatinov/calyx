#include "ecs/GameObject.h"

namespace Calyx {

    GameObject::GameObject(Scene* scene, entt::entity entityID, const String& name)
        : m_scene(scene),
          m_entityID(entityID),
          m_name(name),
          m_components() {}

    void GameObject::AddChild(GameObject* child) {
        CX_CORE_ASSERT(child != nullptr, "GameObject::AddChild called will null child!");
        CX_CORE_ASSERT(m_scene == child->m_scene,
                       "GameObject::AddChild called with GameObject belonging to another scene!");

        m_children.push_back(child);
        child->GetTransform().SetParent(&GetTransform());
        std::erase_if(
            m_scene->m_rootGameObjects,
            [child](GameObject* gameObject) { return *gameObject == *child; }
        );
    }

    void GameObject::SetParent(GameObject* parent) {
        CX_CORE_ASSERT(parent != nullptr, "GameObject::SetParent called will null parent!");
        CX_CORE_ASSERT(m_scene == parent->m_scene,
                       "GameObject::SetParent called with GameObject belonging to another scene!");

        GetTransform().SetParent(&parent->GetTransform());
        parent->m_children.push_back(this);
        std::erase_if(
            m_scene->m_rootGameObjects,
            [this](GameObject* gameObject) {
                return *gameObject == *this;
            }
        );
    }

    Transform& GameObject::GetTransform() {
        auto* tc = GetComponent<TransformComponent>();
        CX_CORE_ASSERT(tc != nullptr, "GameObject has no TransformComponent attached!");
        return tc->GetTransform();
    }

}