#include "ecs/Component.h"
#include "ecs/GameObject.h"

namespace Calyx {

    template<typename T>
    template<typename C>
    C* Component<T>::GetComponent() const {
        return m_gameObject->GetComponent<C>();
    }

}