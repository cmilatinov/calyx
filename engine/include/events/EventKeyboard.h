#pragma once

#include "events/KeyCodes.h"
#include "events/Event.h"

namespace Calyx {

    class CALYX_API EventKey : public Event {
    public:
        KeyCode GetKeyCode() const { return m_keycode; }

        EVENT_CLASS_CATEGORY(EventCategory::Input | EventCategory::Keyboard)

    protected:
        explicit EventKey(KeyCode keycode)
            : m_keycode(keycode) {}

        KeyCode m_keycode;
    };

    class CALYX_API EventKeyPress : public EventKey {
    public:
        explicit EventKeyPress(KeyCode keycode, bool repeated = false)
            : EventKey(keycode), m_repeated(repeated) {}

        bool IsRepeated() const { return m_repeated; }

        String ToString() const override {
            StringStream ss;
            ss << "EventKeyPress: " << m_keycode;
            if (m_repeated)
                ss << " (repeated)";
            return ss.str();
        }

        EVENT_CLASS_TYPE(KeyPress)
    private:
        bool m_repeated;
    };

    class CALYX_API EventKeyRelease : public EventKey {
    public:
        explicit EventKeyRelease(KeyCode keycode)
            : EventKey(keycode) {}

        String ToString() const override {
            StringStream ss;
            ss << "EventKeyRelease: " << m_keycode;
            return ss.str();
        }

        EVENT_CLASS_TYPE(KeyRelease)
    };

    class CALYX_API EventKeyType : public EventKey {
    public:
        explicit EventKeyType(KeyCode keycode)
            : EventKey(keycode) {}

        String ToString() const override {
            StringStream ss;
            ss << "EventKeyType: " << m_keycode;
            return ss.str();
        }

        EVENT_CLASS_TYPE(KeyType)
    };

}