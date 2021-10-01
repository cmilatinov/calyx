#include "core/Application.h"

namespace Calyx {

    Application* Application::s_instance = nullptr;

    Application::Application() {
        CX_CORE_ASSERT(s_instance == nullptr, "Application already running!");
        s_instance = this;

        m_window.reset(Window::Create(WindowMode::DEFAULT));
        m_window->SetEventCallback(CX_BIND_EVENT_FN(OnEvent));
    }

    Application::~Application() {}

    void Application::Run() {
        while (m_running) {
            m_window->OnUpdate();
        }
    }

    void Application::Close() {
        m_running = false;
    }

    void Application::OnEvent(Event& event) {
        CX_DISPATCH_EVENT(EventWindowClose, Application::OnWindowClose, event);
        CX_DISPATCH_EVENT(EventWindowResize, Application::OnWindowResize, event);
    }

    bool Application::OnWindowClose(EventWindowClose& event) {
        Close();
        return true;
    }

    bool Application::OnWindowResize(EventWindowResize& event) {
        return true;
    }



}

