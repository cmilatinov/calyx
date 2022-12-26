#pragma once

#include "ecs/Component.h"
#include "ecs/components/TransformComponent.h"
#include "scene/Scene.h"

namespace Calyx {

    class GameObject {

        friend class Scene;
        friend class Editor::EditorLayer;

        template<typename T, typename ... Args>
        friend constexpr Scope<T> Calyx::CreateScope(Args&& ... args);

    private:
        GameObject(Scene* scene, entt::entity entityID, const String& name);

    public:
        template<typename T, typename ...Ts>
        T* AddComponent(Ts ...args) {
            T& component = m_scene->m_entityRegistry.emplace<T>(m_entityID, std::forward<Ts>(args)...);
            component.m_gameObject = this;
            return &component;
        }

        void AddComponent(const entt::meta_type& type) {
            auto storage = m_scene->m_entityRegistry.storage(type.info().hash());
            storage->emplace(m_entityID);
            void* component = storage->get(m_entityID);
            auto ref = type.from_void(component);
            ref.assign(type.construct());
        }

        template<typename T>
        void RemoveComponent() {
            m_scene->m_entityRegistry.erase<T>(m_entityID);
        }

        template<typename T>
        T* GetComponent() {
            if (HasComponent<T>())
                return &m_scene->m_entityRegistry.template get<T>(m_entityID);
            return nullptr;
        }

        template<typename T>
        bool HasComponent() {
            return m_scene->m_entityRegistry.template all_of<T>(m_entityID);
        }

        void AddChild(GameObject* child);
        void SetParent(GameObject* parent);
        void DetachFromParent();

        const Scene* GetScene() const { return m_scene; }
        String GetName() const { return m_name; }
        String& GetNameRef() { return m_name; }
        const Set<GameObject*>& GetChildren() const { return m_children; }
        GameObject* GetParent() const { return m_parent; }

        Transform& GetTransform();

        friend bool operator==(const GameObject& left, const GameObject& right);
        friend bool operator!=(const GameObject& left, const GameObject& right);

    private:
        Scene* m_scene;
        entt::entity m_entityID = entt::null;
        String m_name;
        GameObject* m_parent = nullptr;
        Set<GameObject*> m_children;

    };

    bool operator==(const GameObject& left, const GameObject& right);
    bool operator!=(const GameObject& left, const GameObject& right);

}