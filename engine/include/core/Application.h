#pragma once

#include "events/Event.h"
#include "events/EventWindow.h"
#include "display/Window.h"

namespace Calyx {

    class CALYX_API Application {

    public:
        Application();
        virtual ~Application();

        virtual void Run();
        virtual void Close();

        static const Application& GetInstance() { return *s_instance; }

    private:
        bool m_running = true;
        Scope<Window> m_window;

        // Singleton instance
        static Application* s_instance;

        // Events
        void OnEvent(Event& event);
        bool OnWindowClose(EventWindowClose& event);
        bool OnWindowResize(EventWindowResize& event);

    };

    extern Application* CreateApplication();

}