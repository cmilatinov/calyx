#include "render/Camera.h"

namespace Calyx {

    Camera::Camera(float fov, float aspect, float nearPlane, float farPlane)
        : m_fov(fov),
          m_aspect(aspect),
          m_nearPlane(nearPlane),
          m_farPlane(farPlane),
          m_projectionMatrix() {
        UpdateProjectionMatrix();
    }

    void Camera::SetFOV(float fov) {
        m_fov = fov;
        UpdateProjectionMatrix();
    }

    void Camera::SetAspect(float aspect) {
        m_aspect = aspect;
        UpdateProjectionMatrix();
    }

    void Camera::SetNearPlane(float nearPlane) {
        m_nearPlane = nearPlane;
        UpdateProjectionMatrix();
    }

    void Camera::SetFarPlane(float farPlane) {
        m_farPlane = farPlane;
        UpdateProjectionMatrix();
    }

    void Camera::UpdateProjectionMatrix() {
        float fovy = 2.0f * atan(tan(glm::radians(m_fov) / 2.0f) * m_aspect);
        m_projectionMatrix = glm::perspective(fovy, m_aspect, m_nearPlane, m_farPlane);
    }

    void Camera::CX_ON_POST_DESERIALIZE() {
        UpdateProjectionMatrix();
    }

}