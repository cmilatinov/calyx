#pragma once

#include "display/WindowMode.h"
#include "events/Event.h"

namespace Calyx {

    /**
     * Describes a window shown at the OS level.
     */
    class CALYX_API Window {

        using EventCallbackFn = std::function<void(Event&)>;

    public:
        virtual ~Window() = default;

        virtual void OnUpdate() = 0;

        virtual uvec2 GetCenter() const = 0;
        virtual uvec2 GetSize() const = 0;
        virtual uint32 GetWidth() const = 0;
        virtual uint32 GetHeight() const = 0;
        virtual void* GetNativeWindow() const = 0;

        virtual void SetTitle(const String& title) = 0;

        void SetEventCallback(const EventCallbackFn& callback) { m_eventCallback = callback; }

        static Window* Create(const WindowMode& mode);
        static Window& GetMainWindow() { return *s_mainWindow; }

    protected:
        EventCallbackFn m_eventCallback;

        static Window* s_mainWindow;

    };

}