#pragma once

#include "events/KeyCodes.h"
#include "events/Event.h"

namespace Calyx {

    class EventKey : public Event {
    public:
        KeyCode GetKeyCode() { return m_keycode; }

        EVENT_CLASS_CATEGORY(CategoryInput | CategoryKeyboard)

    protected:
        explicit EventKey(KeyCode keycode)
            : m_keycode(keycode) {}

        KeyCode m_keycode;
    };

    class EventKeyPress : public EventKey {
    public:
        explicit EventKeyPress(KeyCode keycode, bool repeated = false)
            : EventKey(keycode), m_repeated(repeated) {}

        bool IsRepeated() { return m_repeated; }

        String ToString() const override {
            std::stringstream ss;
            ss << "EventKeyPress: " << m_keycode;
            if (m_repeated)
                ss << " (repeated)";
            return ss.str();
        }

        EVENT_CLASS_TYPE(KeyPress)
    private:
        bool m_repeated;
    };

    class EventKeyRelease : public EventKey {
    public:
        EventKeyRelease(KeyCode keycode)
            : EventKey(keycode) {}

        String ToString() const override {
            std::stringstream ss;
            ss << "EventKeyRelease: " << m_keycode;
            return ss.str();
        }

        EVENT_CLASS_TYPE(KeyRelease)
    };

    class EventKeyType : public EventKey {
    public:
        EventKeyType(KeyCode keycode)
            : EventKey(keycode) {}

        String ToString() const override {
            std::stringstream ss;
            ss << "EventKeyType: " << m_keycode;
            return ss.str();
        }

        EVENT_CLASS_TYPE(KeyType)
    };

}