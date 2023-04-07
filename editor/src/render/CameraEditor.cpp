#include "render/CameraEditor.h"
#include "core/Time.h"
#include "input/Input.h"

namespace Calyx::Editor {

    void CameraEditor::Update() {
        // Translation
        float forward = Input::GetKey(KEY_S) - Input::GetKey(KEY_W);
        float lateral = Input::GetKey(KEY_D) - Input::GetKey(KEY_A);
        float vertical = Input::GetKey(KEY_SPACE) - (Input::GetKey(KEY_LEFT_SHIFT) || Input::GetKey(KEY_RIGHT_SHIFT));
        float speed = (Input::GetKey(KEY_LEFT_CONTROL) || Input::GetKey(KEY_RIGHT_CONTROL)) ? 10.0f : 5.0f;
        vec3 movement = forward * m_transform.Forward() + lateral * m_transform.Right() + vec3(0, vertical, 0);
        m_transform.Translate(Time::StaticDeltaTime<float>() * speed * movement);

        // Rotation
        if (Input::GetMouseButton(MOUSE_BUTTON_1)) {
            constexpr float rotationSpeed = 150.0f;
            vec2 mouseVel = Input::GetMouseVelocity();
            m_transform.Rotate(rotationSpeed * Time::StaticDeltaTime<float>() * vec3(-mouseVel.y, -mouseVel.x, 0));
        }
    }

}