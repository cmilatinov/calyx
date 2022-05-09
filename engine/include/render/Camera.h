#pragma once

#include "math/Transform.h"

namespace Calyx {

    class Camera {

    public:
        virtual Transform& GetTransform() = 0;
        virtual mat4 GetProjectionViewMatrix() const = 0;

    };

}