#include "reflect/ClassRegistry.h"
#include "ecs/Component.h"

namespace Calyx::Reflect {

    CX_SINGLETON_INSTANCE(ClassRegistry);

    ClassRegistry::ClassRegistry() {
        RefreshClassLists();
    }

    void ClassRegistry::RefreshClassLists() {
        using namespace entt::literals;
        auto components = Reflect::Core::GetDerivedClasses<IComponent>();
        m_componentClasses.clear();
        for (const auto& component: components) {
            m_componentClasses.push_back({
                .type = component,
                .functions = {
                    .update = FindFunction(component, CX_UPDATE_HS),
                    .reset = FindFunction(component, CX_RESET_HS),
                    .start = FindFunction(component, CX_START_HS),
                    .destroy = FindFunction(component, CX_DESTROY_HS)
                }
            });
        }
    }

    entt::meta_func ClassRegistry::FindFunction(const entt::meta_type& type, entt::hashed_string name, int arity) {
        auto fn = type.func(name);
        if (!fn || fn.ret() != entt::resolve<void>() || fn.arity() != arity)
            return {};
        return fn;
    }

}