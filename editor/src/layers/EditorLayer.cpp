#include "layers/EditorLayer.h"

namespace Calyx {

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

    void EditorLayer::OnAttach() {
        const Window& window = Application::GetInstance().GetWindow();
        shader = Shader::Create("./assets/shaders/basic.glsl");

        vertexArray = VertexArray::Create();

        indexBuffer = IndexBuffer::Create(sizeof(triangleIndices) / sizeof(uint32), triangleIndices);
        vertexArray->SetIndexBuffer(indexBuffer);

        vertexBuffer = VertexBuffer::Create(sizeof(triangleVertices), triangleVertices);
        vertexBuffer->SetLayout({
            { ShaderDataType::Float3, "pos" },
            { ShaderDataType::Float3, "color" }
        });
        vertexArray->AddVertexBuffer(vertexBuffer);

        msaaFramebuffer = Framebuffer::Create({
            .width = window.GetWidth(),
            .height = window.GetHeight(),
            .samples = 32,
            .attachments = {
                { IRenderTarget::Type::RENDERBUFFER, TextureFormat::DEPTH24_STENCIL8 },
                { IRenderTarget::Type::TEXTURE, TextureFormat::RGBA8 },
            }
        });
        msaaFramebuffer->Unbind();

        framebuffer = Framebuffer::Create({
            .width = window.GetWidth(),
            .height = window.GetHeight(),
            .samples = 1,
            .attachments = {
                { IRenderTarget::Type::RENDERBUFFER, TextureFormat::DEPTH24_STENCIL8 },
                { IRenderTarget::Type::TEXTURE, TextureFormat::RGBA8 },
            }
        });
        framebuffer->Unbind();

        CX_CORE_INFO("Window size: {:d} x {:d}", window.GetWidth(), window.GetHeight());
        RenderCommand::SetViewport(0, 0, window.GetWidth(), window.GetHeight());
        RenderCommand::SetClearColor(vec4(0, 0.2, 0.2, 1));
    }

    void EditorLayer::OnUpdate() {
        msaaFramebuffer->Bind();

        shader->Bind();
        RenderCommand::Clear();
        RenderCommand::DrawIndexed(vertexArray, sizeof(triangleIndices) / sizeof(uint32));

        msaaFramebuffer->Blit(framebuffer, 0, 0);
        msaaFramebuffer->Unbind();

        if (Input::GetKeyDown(KEY_W))
            CX_TRACE("PRESSED W!");

//        if (Input::GetKeyUp(KEY_W))
//            CX_TRACE("RELEASED W!");
    };

    void EditorLayer::OnGUI() {
        // Setup parent window
        const Window& window = Application::GetInstance().GetWindow();
        int windowFlags =
                ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking |
                ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse |
                ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->Pos);
        ImGui::SetNextWindowSize(viewport->Size);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.0f, 0.0f });
        ImGui::Begin("Dockspace Demo", nullptr, windowFlags);
        ImGui::PopStyleVar(3);

        // Setup dock space
        ImGui::DockSpace(ImGui::GetID("DockSpace"), { 0.0f, 0.0f }, ImGuiDockNodeFlags_None);

        // Menubar
        if (ImGui::BeginMenuBar()) {
            if (ImGui::BeginMenu("File")) {
                ImGui::MenuItem("New", "Ctrl+N");
                ImGui::MenuItem("Open...", "Ctrl+O");
                ImGui::MenuItem("Save As...", "Ctrl+Shift+S");
                if (ImGui::MenuItem("Exit", "Alt+F4"))
                    Application::GetInstance().Close();
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }

        // Show example windows
        ImGui::ShowDemoWindow();
        ImGui::ShowMetricsWindow();

        // Show viewport
        ImGui::SetNextWindowSize(ImVec2{ 1280, 720 }, ImGuiCond_FirstUseEver);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.0f, 0.0f });
        ImGui::Begin("Viewport", nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
        ImGui::PopStyleVar();

        uint32 textureID = framebuffer->GetColorAttachment(0).GetRendererID();
        ImGui::Image(reinterpret_cast<void*>(textureID), ImGui::GetContentRegionAvail(), { 0.0f, 1.0f }, { 1.0f, 0.0f });

        ImGui::End();

        ImGui::End();
    }

    void EditorLayer::OnEvent(Event& event) {

    }

}