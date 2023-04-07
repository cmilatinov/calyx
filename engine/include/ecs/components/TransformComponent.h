#pragma once

#include "ecs/Component.h"
#include "math/Transform.h"

namespace Calyx {

    class CALYX_API TransformComponent final : public Component<TransformComponent> {
        CX_BEGIN_REFLECT();

    public:
        String GetDisplayName() const override { return "Transform"; }
        Transform& GetTransform() { return m_transform; }

    private:
        CX_PROPERTY(CX_SERIALIZE)
        Transform m_transform;

    };

}