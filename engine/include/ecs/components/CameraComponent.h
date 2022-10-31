#pragma once

#include "ecs/Component.h"
#include "render/Camera.h"

namespace Calyx {

    class CameraComponent : public Component<CameraComponent> {
        CX_BEGIN_REFLECT();

    public:
        Camera& GetCamera() { return m_camera; }

    private:
        Camera m_camera;

    };

}