#include "input/Input.h"

namespace Calyx {

    Scope<Input> Input::s_instance = nullptr;

    void Input::Init() {
        s_instance = CreateScope<Input>();
    }

    void Input::_OnEvent(Event& event) {
        CX_DISPATCH_EVENT(EventKeyPress, Input::HandleKeyPressEvent, event);
        CX_DISPATCH_EVENT(EventKeyRelease, Input::HandleKeyReleaseEvent, event);
        CX_DISPATCH_EVENT(EventMouseButtonPress, Input::HandleMouseButtonPressEvent, event);
        CX_DISPATCH_EVENT(EventMouseButtonRelease, Input::HandleMouseButtonReleaseEvent, event);
        CX_DISPATCH_EVENT(EventMouseMove, Input::HandleMouseMoveEvent, event);
    }

    void Input::_OnFrameEnd() {
        m_keyDown.clear();
        m_keyUp.clear();
        m_mouseButtonDown.clear();
        m_mouseButtonUp.clear();
        memset(&m_mouseVelocity, 0, sizeof(vec2));
    }

    bool Input::HandleKeyPressEvent(EventKeyPress& event) {
        KeyCode keycode = event.GetKeyCode();
        m_keyDown[keycode] = !m_key[keycode];
        m_key[keycode] = true;
        return false;
    }

    bool Input::HandleKeyReleaseEvent(EventKeyRelease& event) {
        KeyCode keycode = event.GetKeyCode();
        m_keyUp[keycode] = m_key[keycode];
        m_key[keycode] = false;
        return false;
    }

    bool Input::HandleMouseButtonPressEvent(EventMouseButtonPress& event) {
        MouseCode button = event.GetMouseButton();
        m_mouseButtonDown[button] = !m_mouseButton[button];
        m_mouseButton[button] = true;
        return false;
    }

    bool Input::HandleMouseButtonReleaseEvent(EventMouseButtonRelease& event) {
        MouseCode button = event.GetMouseButton();
        m_mouseButtonUp[button] = m_mouseButton[button];
        m_mouseButton[button] = false;
        return false;
    }

    bool Input::HandleMouseMoveEvent(EventMouseMove& event) {
        vec2 mouseCoords(event.GetX(), event.GetY());
        m_mouseVelocity = mouseCoords - m_mouseCoords;
        m_mouseCoords = mouseCoords;
        return false;
    }

}