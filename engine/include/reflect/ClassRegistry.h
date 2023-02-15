#pragma once

namespace Calyx::Reflect {

    class ClassRegistry {
    CX_SINGLETON(ClassRegistry);

    public:
        ClassRegistry();

    public:
        CX_SINGLETON_EXPOSE_METHOD(_GetComponentClasses, const List<entt::meta_type>& GetComponentClasses());

    private:
        void RefreshClassLists();

        const List<entt::meta_type>& _GetComponentClasses() { return m_componentClasses; }

    private:
        List<entt::meta_type> m_componentClasses;

    };

}