#pragma once

namespace Calyx {
    class Application;
    namespace Editor {
        class AssemblyBuilder;
    }
}

namespace Calyx::Reflect {

    class ClassRegistry {
    CX_SINGLETON(ClassRegistry);

        friend class ::Calyx::Application;
        friend class ::Calyx::Editor::AssemblyBuilder;

    public:
        struct ComponentClassInfo {
            entt::meta_type type;
            struct {
                entt::meta_func update;
                entt::meta_func reset;
                entt::meta_func start;
                entt::meta_func destroy;
                entt::meta_func drawGizmos;
                entt::meta_func drawGizmosSelected;
            } functions{};
        };

    private:
        using ComponentMap = Map<entt::id_type, ComponentClassInfo>;

    public:
        ClassRegistry();

    public:
        CX_SINGLETON_EXPOSE_METHOD(_GetComponentClasses, const ComponentMap& GetComponentClasses());

    private:
        void RefreshClassLists();
        entt::meta_func FindFunction(const entt::meta_type& type, entt::hashed_string name, int arity = 0);

        const ComponentMap& _GetComponentClasses() const { return m_componentClasses; }

    private:
        ComponentMap m_componentClasses;

    };

}