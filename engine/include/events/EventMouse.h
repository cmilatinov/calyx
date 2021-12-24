#pragma once

#include "events/MouseCodes.h"
#include "events/Event.h"

namespace Calyx {

    class EventMouseMove : public Event {
    public:
        EventMouseMove(float x, float y)
                : m_mouseX(x), m_mouseY(y) {}

        float GetX() const { return m_mouseX; }
        float GetY() const { return m_mouseY; }

        String ToString() const override {
            std::stringstream ss;
            ss << "EventMouseMove: " << m_mouseX << ", " << m_mouseY;
            return ss.str();
        }

        EVENT_CLASS_TYPE(MouseMove)
        EVENT_CLASS_CATEGORY(EventCategory::Input | EventCategory::Mouse)

    private:
        float m_mouseX, m_mouseY;
    };

    class EventMouseScroll : public Event {
    public:
        EventMouseScroll(float offsetX, float offsetY)
                : m_offsetX(offsetX), m_offsetY(offsetY) {}

        float GetXOffset() const { return m_offsetX; }
        float GetYOffset() const { return m_offsetY; }

        String ToString() const override {
            std::stringstream ss;
            ss << "EventMouseScroll: " << GetXOffset() << ", " << GetYOffset();
            return ss.str();
        }

        EVENT_CLASS_TYPE(MouseScroll)
        EVENT_CLASS_CATEGORY(EventCategory::Input | EventCategory::Mouse)

    private:
        float m_offsetX, m_offsetY;
    };

    class EventMouseButton : public Event {
    public:
        MouseCode GetMouseButton() const { return m_button; }

        EVENT_CLASS_CATEGORY(EventCategory::Input | EventCategory::Mouse | EventCategory::MouseButton)

    protected:
        EventMouseButton(MouseCode button)
                : m_button(button) {}

        MouseCode m_button;
    };

    class EventMouseButtonPress : public EventMouseButton {
    public:
        EventMouseButtonPress(MouseCode button)
                : EventMouseButton(button) {}

        String ToString() const override {
            std::stringstream ss;
            ss << "EventMouseButtonPress: " << m_button;
            return ss.str();
        }

        EVENT_CLASS_TYPE(MouseButtonPress)
    };

    class EventMouseButtonRelease : public EventMouseButton {
    public:
        EventMouseButtonRelease(MouseCode button)
                : EventMouseButton(button) {}

        String ToString() const override {
            std::stringstream ss;
            ss << "EventMouseButtonRelease: " << m_button;
            return ss.str();
        }

        EVENT_CLASS_TYPE(MouseButtonRelease)
    };

}
