#pragma once

#include "events/EventMouse.h"
#include "events/EventKeyboard.h"

namespace Calyx {

    class Input {

    public:
        static void Init();

        static void OnEvent(Event& event) { s_instance->_OnEvent(event); }
        static void OnFrameEnd() { s_instance->_OnFrameEnd(); };

        static bool GetKey(KeyCode key) { return s_instance->_GetKey(key); }
        static bool GetKeyUp(KeyCode key) { return s_instance->_GetKeyDown(key); }
        static bool GetKeyDown(KeyCode key) { return s_instance->_GetKeyUp(key); }

        static bool GetMouseButton(MouseCode button) { return s_instance->_GetMouseButton(button); }
        static bool GetMouseButtonDown(MouseCode button) { return s_instance->_GetMouseButtonDown(button); }
        static bool GetMouseButtonUp(MouseCode button) { return s_instance->_GetMouseButtonUp(button); }
        static vec2 GetMousePosition() { return s_instance->_GetMousePosition(); }
        static vec2 GetMouseVelocity() { return s_instance->_GetMouseVelocity(); }

    private:
        static Scope<Input> s_instance;

        Map<KeyCode, bool> m_key;
        Map<KeyCode, bool> m_keyDown;
        Map<KeyCode, bool> m_keyUp;

        Map<MouseCode, bool> m_mouseButton;
        Map<MouseCode, bool> m_mouseButtonDown;
        Map<MouseCode, bool> m_mouseButtonUp;

        vec2 m_mouseCoords;
        vec2 m_mouseVelocity;

        void _OnEvent(Event& event);
        void _OnFrameEnd();

        bool HandleKeyPressEvent(EventKeyPress& event);
        bool HandleKeyReleaseEvent(EventKeyRelease& event);

        bool HandleMouseButtonPressEvent(EventMouseButtonPress& event);
        bool HandleMouseButtonReleaseEvent(EventMouseButtonRelease& event);

        bool HandleMouseMoveEvent(EventMouseMove& event);

        bool _GetMouseButton(MouseCode button) { return m_mouseButton[button]; }
        bool _GetMouseButtonDown(MouseCode button) { return m_mouseButtonDown[button]; }
        bool _GetMouseButtonUp(MouseCode button) { return m_mouseButtonUp[button]; }
        vec2 _GetMousePosition() { return m_mouseCoords; }
        vec2 _GetMouseVelocity() { return m_mouseVelocity; }

        bool _GetKey(KeyCode key) { return m_key[key]; }
        bool _GetKeyUp(KeyCode key) { return m_keyDown[key]; }
        bool _GetKeyDown(KeyCode key) { return m_keyUp[key]; }

    };

}