#pragma once

#include "ecs/Component.h"
#include "render/Camera.h"

namespace Calyx {

    class CALYX_API CameraComponent final : public Component<CameraComponent> {
        CX_BEGIN_REFLECT();

    public:
        Camera& GetCamera() { return m_camera; }

        String GetDisplayName() const override { return "Camera Component"; }

    private:
        CX_PROPERTY(CX_SERIALIZE)
        Camera m_camera;

    };

}