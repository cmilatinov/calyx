#pragma once

#include "ecs/Component.h"
#include "ecs/components/TransformComponent.h"
#include "scene/Scene.h"

namespace Calyx {

    class GameObject {

        friend class Scene;
        template<typename T, typename ... Args>
        friend constexpr Scope<T> Calyx::CreateScope(Args&& ... args);

    private:
        GameObject(Scene* scene, entt::entity entityID, const String& name);

    public:
        template<typename T, typename ...Ts>
        T* AddComponent(Ts ...args) {
            T& component = m_scene->m_entityRegistry.emplace<T>(m_entityID, args...);
            component.m_gameObject = this;
            m_components.push_back(&component);
            return &component;
        }

        template<typename T>
        bool RemoveComponent() {
            size_t typeId = typeid(T).hash_code();
            auto component = std::erase_if(m_components, [typeId](IComponent* comp) {
                return comp->GetTypeId() == typeId;
            });
            return component != m_components.end();
        }

        template<typename T>
        T* GetComponent() {
            size_t typeId = typeid(T).hash_code();
            auto component = std::find_if(m_components.begin(), m_components.end(),
                                          [typeId](IComponent* comp) {
                                              return comp->GetTypeId() == typeId;
                                          });
            if (component != m_components.end())
                return reinterpret_cast<T*>(*component);
            return nullptr;
        }

        template<typename T>
        bool HasComponent() {
            return GetComponent<T>() != nullptr;
        }

        void AddChild(GameObject* child);
        void SetParent(GameObject* parent);

        const Scene* GetScene() { return m_scene; }
        String GetName() const { return m_name; }
        String& GetNameRef() { return m_name; }
        const List<GameObject*>& GetChildren() { return m_children; }

        Transform& GetTransform();

        bool operator==(const GameObject& other) { return m_entityID == other.m_entityID; }
        bool operator!=(const GameObject& other) { return m_entityID != other.m_entityID; }

    private:
        Scene* m_scene;
        entt::entity m_entityID = entt::null;
        String m_name;
        List<IComponent*> m_components;
        List<GameObject*> m_children;

    };

}