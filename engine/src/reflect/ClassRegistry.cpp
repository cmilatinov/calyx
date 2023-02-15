#include "reflect/ClassRegistry.h"
#include "ecs/Component.h"

namespace Calyx::Reflect {

    CX_SINGLETON_INSTANCE(ClassRegistry);

    ClassRegistry::ClassRegistry() {
        RefreshClassLists();
    }

    void ClassRegistry::RefreshClassLists() {
        m_componentClasses = Reflect::Core::GetDerivedClasses<IComponent>();
    }

}