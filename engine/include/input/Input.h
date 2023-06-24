#pragma once

#include "events/EventMouse.h"
#include "events/EventKeyboard.h"

namespace Calyx {

    class Application;

    class CALYX_API Input {
    CX_SINGLETON(Input);

        friend class ::Calyx::Application;

    public:
        CX_SINGLETON_EXPOSE_METHOD(_OnEvent, void OnEvent(Event& event), event);
        CX_SINGLETON_EXPOSE_METHOD(_OnFrameEnd, void OnFrameEnd());

        CX_SINGLETON_EXPOSE_METHOD(_GetKey, bool GetKey(KeyCode key), key);
        CX_SINGLETON_EXPOSE_METHOD(_GetKeyUp, bool GetKeyUp(KeyCode key), key);
        CX_SINGLETON_EXPOSE_METHOD(_GetKeyDown, bool GetKeyDown(KeyCode key), key);

        CX_SINGLETON_EXPOSE_METHOD(_GetMouseButton, bool GetMouseButton(MouseCode button), button);
        CX_SINGLETON_EXPOSE_METHOD(_GetMouseButtonDown, bool GetMouseButtonDown(MouseCode button), button);
        CX_SINGLETON_EXPOSE_METHOD(_GetMouseButtonUp, bool GetMouseButtonUp(MouseCode button), button);
        CX_SINGLETON_EXPOSE_METHOD(_GetMousePosition, vec2 GetMousePosition());
        CX_SINGLETON_EXPOSE_METHOD(_GetMouseVelocity, vec2 GetMouseVelocity());

    public:
        Input() = default;

    private:
        Map<KeyCode, bool> m_key;
        Map<KeyCode, bool> m_keyDown;
        Map<KeyCode, bool> m_keyUp;

        Map<MouseCode, bool> m_mouseButton;
        Map<MouseCode, bool> m_mouseButtonDown;
        Map<MouseCode, bool> m_mouseButtonUp;

        vec2 m_mouseCoords{};
        vec2 m_mouseVelocity{};

        void _OnEvent(Event& event);
        void _OnFrameEnd();

        bool HandleKeyPressEvent(EventKeyPress& event);
        bool HandleKeyReleaseEvent(EventKeyRelease& event);

        bool HandleMouseButtonPressEvent(EventMouseButtonPress& event);
        bool HandleMouseButtonReleaseEvent(EventMouseButtonRelease& event);

        bool HandleMouseMoveEvent(EventMouseMove& event);

        bool _GetKey(KeyCode key) { return m_key[key]; }
        bool _GetKeyUp(KeyCode key) { return m_keyDown[key]; }
        bool _GetKeyDown(KeyCode key) { return m_keyUp[key]; }

        bool _GetMouseButton(MouseCode button) { return m_mouseButton[button]; }
        bool _GetMouseButtonDown(MouseCode button) { return m_mouseButtonDown[button]; }
        bool _GetMouseButtonUp(MouseCode button) { return m_mouseButtonUp[button]; }
        vec2 _GetMousePosition() { return m_mouseCoords; }
        vec2 _GetMouseVelocity() { return m_mouseVelocity; }

    };

}