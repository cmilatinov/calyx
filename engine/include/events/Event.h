#pragma once

#define EVENT_CLASS_TYPE(type)  static EventType GetStaticType() { return EventType::type; }\
								virtual EventType GetEventType() const override { return GetStaticType(); }\
								virtual const char* GetName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) virtual uint32 GetCategoryFlags() const override { return category; }

namespace Calyx {

    enum EventType {
        ApplicationRender, ApplicationUpdate,
        WindowClose, WindowMove, WindowResize, WindowFocus, WindowLostFocus, WindowMinimize, WindowMaximize, WindowRestore,
        KeyPress, KeyRelease, KeyType,
        MouseButtonPress, MouseButtonRelease, MouseMove, MouseScroll
    };

    enum EventCategory {
        None                    = 0,
        CategoryApplication     = BIT(0),
        CategoryWindow          = BIT(1),
        CategoryInput           = BIT(2),
        CategoryMouse           = BIT(3),
        CategoryMouseButton     = BIT(4),
        CategoryKeyboard        = BIT(5),
    };

    class CALYX_API Event {

    public:
        bool handled = false;

        virtual const char* GetName() const = 0;
        virtual EventType GetEventType() const = 0;
        virtual uint32 GetCategoryFlags() const = 0;
        virtual String ToString() const { return GetName(); }

        bool IsInCategory(EventCategory category) const { return GetCategoryFlags() & static_cast<uint32>(category); }

    };

    template<class T, typename F>
    bool DispatchEvent(Event& event, const F& func) {
        if (event.GetEventType() == T::GetStaticType()) {
            event.handled |= func(static_cast<T&>(event));
            return true;
        }
        return false;
    }

}
