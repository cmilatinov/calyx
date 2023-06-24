#pragma once

#include "scene/Scene.h"
#include "reflect/ClassRegistry.h"

namespace Calyx {

    class IComponent;
    template<typename T>
    class Component;

    class SceneExporter;
    class SceneImporter;

    namespace Editor {
        class EditorLayer;
        class InspectorPanel;
    }

    class CALYX_API GameObject {

        friend class Scene;
        friend class SceneExporter;
        friend class SceneImporter;
        friend class Editor::EditorLayer;
        friend class Editor::InspectorPanel;

        template<typename T>
        friend
        class Component;

        template<typename T, typename ... Args>
        friend constexpr Scope<T> Calyx::CreateScope(Args&& ... args);

    private:
        GameObject(Scene* scene, UUID id, entt::entity entityID, const String& name);

    public:
        template<typename T, typename ...Args>
        T* AddComponent(Args&& ... args) {
            static_assert(std::is_base_of_v<IComponent, T>, "T must extend IComponent!");
            T& component = m_scene->m_entityRegistry.emplace<T>(m_entityID, std::forward<Args>(args)...);
            component.m_gameObject = this;
            return &component;
        }

        entt::meta_any AddComponent(const entt::meta_type& type, const entt::meta_any& src = entt::meta_any());

        template<typename T>
        void RemoveComponent() {
            m_scene->m_entityRegistry.erase<T>(m_entityID);
        }

        void RemoveComponent(const entt::meta_type& type);

        template<typename T>
        T* GetComponent() {
            if (HasComponent<T>())
                return &m_scene->m_entityRegistry.get<T>(m_entityID);
            return nullptr;
        }

        entt::meta_any GetComponent(const entt::meta_type& type);

        template<typename T>
        bool HasComponent() {
            return m_scene->m_entityRegistry.all_of<T>(m_entityID);
        }

        bool HasComponent(const entt::meta_type& type);

        void AddChild(GameObject* child);
        void SetParent(GameObject* parent);
        void DetachFromParent();

        void SetName(const String& name) { m_name = name; }

        const UUID& GetID() const { return m_id; }

        const Scene* GetScene() const { return m_scene; }
        const String& GetName() const { return m_name; }
        const Set<GameObject*>& GetChildren() const { return m_children; }
        GameObject* GetParent() const { return m_parent; }

        Transform& GetTransform();

        friend bool operator==(const GameObject& left, const GameObject& right);
        friend bool operator!=(const GameObject& left, const GameObject& right);

    private:
        Scene* m_scene;
        UUID m_id;
        entt::entity m_entityID = entt::null;
        String m_name;
        GameObject* m_parent = nullptr;
        Set<GameObject*> m_children;

    };

    bool operator==(const GameObject& left, const GameObject& right);
    bool operator!=(const GameObject& left, const GameObject& right);

}