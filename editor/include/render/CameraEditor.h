#pragma once

#include "render/Camera.h"

namespace Calyx::Editor {

    class CameraEditor : public Camera {

    public:
        void Update();

        Transform& GetTransform() { return m_transform; };
        mat4 GetProjectionViewMatrix() { return m_projectionMatrix * m_transform.GetInverseMatrix(); }

    private:
        Transform m_transform;

    };

}