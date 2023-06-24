#pragma once

#include "core/Time.h"
#include "core/LayerStack.h"
#include "events/Event.h"
#include "events/EventWindow.h"
#include "display/Window.h"

namespace Calyx {

    class GuiLayer;

    class CALYX_API Application {

    public:
        explicit Application(const WindowMode& windowMode = WindowMode::DEFAULT);
        virtual ~Application();

        virtual void Run();
        virtual void Close();

        void PushOverlay(ILayer* layer);
        void PushUnderlay(ILayer* layer);
        void EmplaceLayer(uint32 index, ILayer* layer);
        void PopLayer(ILayer* layer);

        const Window& GetWindow() const { return *m_window; }

        static Application& GetInstance() { return *s_instance; }

    private:
        void Init();
        void Destroy();

        void OnEvent(Event& event);
        bool OnWindowClose(EventWindowClose& event);
        bool OnWindowResize(EventWindowResize& event);

    private:
        static Application* s_instance;

        bool m_running = true;
        Scope<Window> m_window;
        Scope<LayerStack> m_layerStack;
        GuiLayer* m_guiLayer = nullptr;

    };

    extern Scope<Application> CreateApplication(int argc, char** argv);

}