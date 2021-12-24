#include "core/Application.h"
#include "layers/GuiLayer.h"

#include "render/Renderer.h"

namespace Calyx {

    Application* Application::s_instance = nullptr;

    Application::Application()
        : m_window(Window::Create(WindowMode::DEFAULT)) {
        // Singleton application instance
        CX_CORE_ASSERT(s_instance == nullptr, "Application already running!");
        s_instance = this;

        // Window event callback
        m_window->SetEventCallback(CX_BIND_EVENT_FN(OnEvent));

        // Initialize renderer
        Renderer::Init();

        // Gui layer
        m_guiLayer = new GuiLayer();
        m_layerStack.PushOverlay(m_guiLayer);
    }

    void Application::Run() {
        while (m_running) {
            for (auto* layer : m_layerStack) {
                layer->OnUpdate();
            }

            m_guiLayer->Begin();
            for (auto* layer : m_layerStack) {
                layer->OnGUI();
            }
            m_guiLayer->End();

            m_window->OnUpdate();
        }
    }

    void Application::Close() {
        m_running = false;
    }

    void Application::OnEvent(Event& event) {
        CX_DISPATCH_EVENT(EventWindowClose, Application::OnWindowClose, event);
        CX_DISPATCH_EVENT(EventWindowResize, Application::OnWindowResize, event);

        for (auto it = m_layerStack.rbegin(); it != m_layerStack.rend(); ++it) {
            if (event.handled)
                break;
            (*it)->OnEvent(event);
        }
    }

    bool Application::OnWindowClose(EventWindowClose& event) {
        Close();
        event.handled = true;
        return true;
    }

    bool Application::OnWindowResize(EventWindowResize& event) {
        return true;
    }

    void Application::PushOverlay(ILayer* layer) {
        m_layerStack.PushOverlay(layer);
    }

    void Application::PushUnderlay(ILayer* layer) {
        m_layerStack.PushUnderlay(layer);
    }

    void Application::EmplaceLayer(uint32 index, ILayer* layer) {
        m_layerStack.EmplaceLayer(index, layer);
    }

    void Application::PopLayer(ILayer* layer) {
        m_layerStack.PopLayer(layer);
    }

}

