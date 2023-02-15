#include "core/Application.h"

#include <iostream>
#include <ranges>

#include "layers/GuiLayer.h"
#include "render/Renderer.h"
#include "input/Input.h"
#include "reflect/ClassRegistry.h"
#include "assets/AssetRegistry.h"
#include "serialization/Serializer.h"

namespace Calyx {

    Application* Application::s_instance = nullptr;

    Application::Application()
        : m_window(Window::Create(WindowMode::DEFAULT)) {
        // Singleton application instance
        CX_CORE_ASSERT(s_instance == nullptr, "Application already running!");
        s_instance = this;

        Init();
    }

    Application::Application(const WindowMode& windowMode)
        : m_window(Window::Create(windowMode)) {
        // Singleton application instance
        CX_CORE_ASSERT(s_instance == nullptr, "Application already running!");
        s_instance = this;

        Init();
    }

    void Application::Init() {
        // Window event callback
        m_window->SetEventCallback(CX_BIND_EVENT_METHOD(OnEvent));

        // Initialize renderer
        Renderer::Init();

        // Initialize input
        Input::Init();

        // Initialize registries
        ClassRegistry::Init();
        AssetRegistry::Init();
        Serializer::Init();

        // Gui layer
        m_guiLayer = new GuiLayer();
        m_layerStack.PushOverlay(m_guiLayer);
    }

    void Application::Run() {
        Time::Init();
        while (m_running) {
            // Time measurements
            Time::GetInstance().UpdateTime();

            // Window events
            m_window->OnUpdate();

            // Layers
            for (auto* layer: m_layerStack) {
                layer->OnUpdate();
            }

            // Layer GUIs
            m_guiLayer->Begin();
            for (auto* layer: m_layerStack) {
                layer->OnGUI();
            }
            m_guiLayer->End();

            // Input
            Input::OnFrameEnd();
        }
    }

    void Application::Close() {
        m_running = false;
    }

    void Application::OnEvent(Event& event) {
        CX_DISPATCH_EVENT(EventWindowClose, Application::OnWindowClose, event);
        CX_DISPATCH_EVENT(EventWindowResize, Application::OnWindowResize, event);

        Input::OnEvent(event);

        for (auto& i_layer: std::ranges::reverse_view(m_layerStack)) {
            if (event.handled)
                break;
            i_layer->OnEvent(event);
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

