#pragma once

#define EVENT_CLASS_TYPE(type)  static EventType::T GetStaticType() { return EventType::type; }\
                                virtual EventType::T GetEventType() const override { return GetStaticType(); }\
                                virtual const char* GetName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) virtual uint32 GetCategoryFlags() const override { return category; }

namespace Calyx {

    namespace EventType {
        enum T {
            ApplicationRender,
            ApplicationUpdate,
            WindowClose,
            WindowMove,
            WindowResize,
            WindowFocus,
            WindowLostFocus,
            WindowMinimize,
            WindowMaximize,
            WindowRestore,
            KeyPress,
            KeyRelease,
            KeyType,
            MouseButtonPress,
            MouseButtonRelease,
            MouseMove,
            MouseScroll
        };
    }

    namespace EventCategory {
        enum T {
            None = 0,
            Application = BIT(0),
            Window = BIT(1),
            Input = BIT(2),
            Mouse = BIT(3),
            MouseButton = BIT(4),
            Keyboard = BIT(5),
        };
    }

    class CALYX_API Event {

    public:
        bool handled = false;

        virtual const char* GetName() const = 0;
        virtual EventType::T GetEventType() const = 0;
        virtual uint32 GetCategoryFlags() const = 0;
        virtual String ToString() const { return GetName(); }

        bool IsInCategory(EventCategory::T category) const {
            return GetCategoryFlags() & static_cast<uint32>(category);
        }

        template<class T, typename F>
        bool Dispatch(const F& func) {
            if (GetEventType() == T::GetStaticType()) {
                handled |= func(static_cast<T&>(*this));
                return true;
            }
            return false;
        }

    };

}
