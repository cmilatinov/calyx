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
    }

    void Input::_OnFrameEnd() {
        for (auto& [_, v] : m_keyDown)
            v = false;
        for (auto& [_, v] : m_keyUp)
            v = false;
        for (auto& [_, v] : m_mouseButtonDown)
            v = false;
        for (auto& [_, v] : m_mouseButtonUp)
            v = false;
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

}