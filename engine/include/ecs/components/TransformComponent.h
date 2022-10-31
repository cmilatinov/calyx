#pragma once

#include "ecs/Component.h"
#include "math/Transform.h"

namespace Calyx {

    class TransformComponent : public Component<TransformComponent> {
        CX_BEGIN_REFLECT();

    public:
        String GetName() const override { return "Transform"; }
        Transform& GetTransform() { return m_transform; }

    private:
        CX_REFLECT_SERIALIZE Transform m_transform;

    };

}