#pragma once

#include "events/Event.h"

namespace Calyx {

    // WindowClose, WindowMove, WindowResize, WindowFocus, WindowLostFocus, WindowMinimize, WindowMaximize,

    class EventWindow : public Event {

    public:
        EVENT_CLASS_CATEGORY(CategoryWindow)

    };

    class EventWindowClose : public EventWindow {

    public:
        EVENT_CLASS_TYPE(WindowClose)

    };

    class EventWindowMove : public EventWindow {

    public:
        EventWindowMove(int posX, int posY)
            : EventWindow(), m_posX(posX), m_posY(posY) {}

        inline int GetPosX() const { return m_posX; }
        inline int GetPosY() const { return m_posY; }

        String ToString() const override {
            std::stringstream ss;
            ss << "EventWindowMove: " << m_posX << ", " << m_posY;
            return ss.str();
        }

        EVENT_CLASS_TYPE(WindowMove)

    private:
        const int m_posX, m_posY;
    };

    class EventWindowResize : public EventWindow {

    public:
        EventWindowResize(int width, int height)
            : EventWindow(), m_width(width), m_height(height) {}

        inline int GetWidth() const { return m_width; }
        inline int GetHeight() const { return m_height; }

        String ToString() const override {
            std::stringstream ss;
            ss << "EventWindowResize: " << m_width << ", " << m_height;
            return ss.str();
        }

        EVENT_CLASS_TYPE(WindowResize)

    private:
        const int m_width, m_height;
    };

    class EventWindowFocus : public EventWindow {

    public:
        EVENT_CLASS_TYPE(WindowFocus)

    };

    class EventWindowLostFocus : public EventWindow {

    public:
        EVENT_CLASS_TYPE(WindowLostFocus)

    };

    class EventWindowMinimize : public EventWindow {

    public:
        EVENT_CLASS_TYPE(WindowMinimize)

    };

    class EventWindowMaximize : public EventWindow {

    public:
        EVENT_CLASS_TYPE(WindowMaximize)

    };

    class EventWindowRestore : public EventWindow {

    public:
        EVENT_CLASS_TYPE(WindowRestore)

    };

}

