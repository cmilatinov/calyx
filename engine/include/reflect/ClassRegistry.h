#pragma once

namespace Calyx::Editor {
    class AssemblyBuilder;
}

namespace Calyx::Reflect {

    class ClassRegistry {
    CX_SINGLETON(ClassRegistry);

        friend class Editor::AssemblyBuilder;

    public:
        struct ComponentClassInfo {
            entt::meta_type type;
            struct {
                entt::meta_func update;
                entt::meta_func reset;
                entt::meta_func start;
                entt::meta_func destroy;
            } functions{};
        };

    public:
        ClassRegistry();

    public:
        CX_SINGLETON_EXPOSE_METHOD(_GetComponentClasses, const List<ComponentClassInfo>& GetComponentClasses());

    private:
        void RefreshClassLists();
        entt::meta_func FindFunction(const entt::meta_type& type, entt::hashed_string name, int arity = 0);

        const List<ComponentClassInfo>& _GetComponentClasses() const { return m_componentClasses; }

    private:
        List<ComponentClassInfo> m_componentClasses;

    };

}