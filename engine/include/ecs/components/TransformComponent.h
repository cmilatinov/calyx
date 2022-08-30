#pragma once

#include "ecs/Component.h"
#include "math/Transform.h"

namespace Calyx {

    class TransformComponent : public Component<TransformComponent> {

    public:
        Transform& GetTransform() { return m_transform; }

    private:
        Transform m_transform;

    };

}