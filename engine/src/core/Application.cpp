#include "core/Application.h"

#include "render/Renderer.h"
#include "render/objects/Shader.h"
#include "render/objects/Framebuffer.h"

namespace Calyx {

    Application* Application::s_instance = nullptr;

    const uint32 triangleIndices[] = {
        0, 1, 3,
        1, 2, 3
    };

    const float triangleVertices[] = {
        0.5f, 0.6f, 0.0f,       1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.0f,      0.0f, 1.0f, 0.0f,
        -0.5f, -0.6f, 0.0f,     0.0f, 0.0f, 1.0f,
        -0.5f, 0.5f, 0.0f,      0.0f, 0.0f, 0.0f
    };

    Application::Application()
        : m_window(Window::Create(WindowMode::DEFAULT)) {
        // Singleton application instance
        CX_CORE_ASSERT(s_instance == nullptr, "Application already running!");
        s_instance = this;

        // Window event callback
        m_window->SetEventCallback(CX_BIND_EVENT_FN(OnEvent));

        // Initialize renderer
        Renderer::Init();
    }

    void Application::Run() {
        Ref<Shader> shader = Shader::Create("./assets/shaders/basic.glsl");

        Ref<VertexArray> vertexArray = VertexArray::Create();

        Ref<IndexBuffer> indexBuffer = IndexBuffer::Create(sizeof(triangleIndices) / sizeof(uint32), triangleIndices);
        vertexArray->SetIndexBuffer(indexBuffer);

        Ref<VertexBuffer> vertexBuffer = VertexBuffer::Create(sizeof(triangleVertices), triangleVertices);
        vertexBuffer->SetLayout({
            { ShaderDataType::Float3, "pos" },
            { ShaderDataType::Float3, "color" }
        });
        vertexArray->AddVertexBuffer(vertexBuffer);

        Ref<Framebuffer> framebuffer = Framebuffer::Create({
            .width = m_window->GetWidth(),
            .height = m_window->GetHeight(),
            .samples = 32,
            .attachments = {
                { IRenderTarget::Type::RENDERBUFFER, TextureFormat::DEPTH24_STENCIL8 },
                { IRenderTarget::Type::TEXTURE, TextureFormat::RGBA8 },
            }
        });

        CX_CORE_INFO("Window size: {:d} x {:d}", m_window->GetWidth(), m_window->GetHeight());
        RenderCommand::SetViewport(0, 0, m_window->GetWidth(), m_window->GetHeight());
        RenderCommand::SetClearColor(vec4(0, 0.2, 0.2, 1));

        while (m_running) {
            framebuffer->Bind();

            shader->Bind();
            RenderCommand::Clear();
            RenderCommand::DrawIndexed(vertexArray, sizeof(triangleIndices) / sizeof(uint32));

            framebuffer->Unbind();

            framebuffer->Blit(nullptr, 0, 0);

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
        event.handled = true;
        return true;
    }

    bool Application::OnWindowResize(EventWindowResize& event) {
        return true;
    }

}

