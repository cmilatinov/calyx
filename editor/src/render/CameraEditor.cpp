#include "render/CameraEditor.h"
#include "core/Time.h"
#include "input/Input.h"

namespace Calyx {

    void CameraEditor::Update() {
        float forward = Input::GetKey(KEY_W) - Input::GetKey(KEY_S);
        float lateral = Input::GetKey(KEY_D) - Input::GetKey(KEY_A);
        float vertical = (Input::GetKey(KEY_LEFT_SHIFT) || Input::GetKey(KEY_RIGHT_SHIFT)) - Input::GetKey(KEY_SPACE);
        vec3 movement(lateral, vertical, forward);
    }

    void CameraEditor::SetFOV(float fov) {
        m_fovX = fov;
        UpdateProjectionMatrix();
    }

    void CameraEditor::SetNearPlane(float nearPlane) {
        m_nearPlane = nearPlane;
        UpdateProjectionMatrix();
    }

    void CameraEditor::SetFarPlane(float farPlane) {
        m_farPlane = farPlane;
        UpdateProjectionMatrix();
    }

    void CameraEditor::UpdateProjectionMatrix() {
        float aspect = (float)Window::GetMainWindow().GetWidth() / (float)Window::GetMainWindow().GetHeight();
        float fovY = 2.0f * std::atan(std::tan(m_fovX / 2.0f) / aspect);
        m_projectionMatrix = glm::perspective(fovY, aspect, m_nearPlane, m_farPlane);
    }

}