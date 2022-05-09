#pragma once

#include "render/Camera.h"

namespace Calyx {

    class CameraEditor : Camera {

    public:
        void Update();

        Transform& GetTransform() { return m_transform; };
        mat4 GetProjectionViewMatrix() const { return m_projectionMatrix * m_transform.GetInverseMatrix(); }
        float GetFOV() const { return m_fovX; }
        float GetNearPlane() const { return m_nearPlane; }
        float GetFarPlane() const { return m_farPlane; }

        void SetFOV(float fov);
        void SetNearPlane(float nearPlane);
        void SetFarPlane(float farPlane);

    private:
        float m_fovX;
        float m_nearPlane;
        float m_farPlane;
        mat4 m_projectionMatrix;
        Transform m_transform;

        void UpdateProjectionMatrix();

    };

}