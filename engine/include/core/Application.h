#pragma once

#include "core/LayerStack.h"
#include "events/Event.h"
#include "events/EventWindow.h"
#include "display/Window.h"

namespace Calyx {

    class GuiLayer;

    class CALYX_API Application {

    public:
        Application();
        virtual ~Application() = default;

        virtual void Run();
        virtual void Close();

        void PushOverlay(ILayer* layer);
        void PushUnderlay(ILayer* layer);
        void EmplaceLayer(uint32 index, ILayer* layer);
        void PopLayer(ILayer* layer);

        const Window& GetWindow() const { return *m_window; }

        static Application& GetInstance() { return *s_instance; }

    private:
        bool m_running = true;
        Scope<Window> m_window;
        LayerStack m_layerStack;
        GuiLayer* m_guiLayer;

        // Singleton instance
        static Application* s_instance;

        // Events
        void OnEvent(Event& event);
        bool OnWindowClose(EventWindowClose& event);
        bool OnWindowResize(EventWindowResize& event);

    };

    extern Application* CreateApplication();

}