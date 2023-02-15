#pragma once

#include "math/Transform.h"

namespace Calyx {

    class CALYX_API Camera {
        CX_BEGIN_REFLECT();

    public:
        explicit Camera(
            float fov = 70.0f,
            float aspect = 16.0f / 9.0f,
            float nearPlane = 0.01f,
            float farPlane = 100.0f
        );

        mat4 GetProjectionMatrix() const { return m_projectionMatrix; }

        float GetFOV() const { return m_fov; }
        float GetAspect() const { return m_aspect; }
        float GetNearPlane() const { return m_nearPlane; }
        float GetFarPlane() const { return m_farPlane; }

        float* GetFOVPtr() { return &m_fov; }
        float* GetAspectPtr() { return &m_aspect; }
        float* GetNearPlanePtr() { return &m_nearPlane; }
        float* GetFarPlanePtr() { return &m_farPlane; }

        void SetFOV(float fov);
        void SetAspect(float aspect);
        void SetNearPlane(float nearPlane);
        void SetFarPlane(float farPlane);

        void Update() { return UpdateProjectionMatrix(); }

    protected:
        CX_PROPERTY(CX_SERIALIZE)
        float m_fov;
        CX_PROPERTY(CX_SERIALIZE)
        float m_aspect;
        CX_PROPERTY(CX_SERIALIZE)
        float m_nearPlane;
        CX_PROPERTY(CX_SERIALIZE)
        float m_farPlane;

        mat4 m_projectionMatrix;

    private:
        void UpdateProjectionMatrix();

    };

}