#include "layers/GuiLayer.h"

#include <IconsMaterialDesign.h>

namespace Calyx {

    void GuiLayer::OnAttach() {
        // ImGui Setup
        IMGUI_CHECKVERSION();
        m_context = ImGui::CreateContext();
        ImPlot::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
        io.ConfigWindowsMoveFromTitleBarOnly = true;
        //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

        // Setup Dear ImGui style
        SetupImGuiTheme();

        // Init ImGui for OpenGL 3.0
        ImGui_ImplGlfw_InitForOpenGL(
            static_cast<GLFWwindow*>(Application::GetInstance().GetWindow().GetNativeWindow()),
            true
        );
        ImGui_ImplOpenGL3_Init("#version 400");
    }

    void GuiLayer::OnDetach() {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImPlot::DestroyContext();
        ImGui::DestroyContext();
    }

    void GuiLayer::OnEvent(Event& event) {
//        ImGuiIO& io = ImGui::GetIO();
//        event.handled |= event.IsInCategory(EventCategory::Mouse) & io.WantCaptureMouse;
//        event.handled |= event.IsInCategory(EventCategory::Keyboard) & io.WantCaptureKeyboard;
    }

    void GuiLayer::Begin() {
        ImGui::SetCurrentContext(m_context);
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGuizmo::BeginFrame();
    }

    void GuiLayer::End() {
        ImGui::SetCurrentContext(m_context);
        ImGuiIO& io = ImGui::GetIO();
        const Application& app = Application::GetInstance();
        io.DisplaySize = ImVec2((float)app.GetWindow().GetWidth(), (float)app.GetWindow().GetHeight());

        // Rendering
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Update viewports and reset context
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            GLFWwindow* currentContext = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(currentContext);
        }
    }

    void GuiLayer::SetupImGuiTheme() {
        // Add fonts
        const float fontSizeNormal = 22.0f;
        const float fontSizeSmall = 18.0f;
        const float fontSizeLarge = 30.0f;
        static const ImWchar iconRanges[] = { 1, ICON_MAX_16_MD, 0 };
        auto& io = ImGui::GetIO();
        io.Fonts->AddFontFromFileTTF("assets/fonts/inconsolata/Inconsolata-Regular.ttf", fontSizeSmall, nullptr, iconRanges);
        io.Fonts->AddFontFromFileTTF("assets/fonts/inconsolata/Inconsolata-Regular.ttf", fontSizeNormal, nullptr, iconRanges);
        io.Fonts->AddFontFromFileTTF("assets/fonts/inconsolata/Inconsolata-Regular.ttf", fontSizeLarge, nullptr, iconRanges);
        io.Fonts->AddFontFromFileTTF("assets/fonts/inconsolata/Inconsolata-Bold.ttf", fontSizeSmall, nullptr, iconRanges);
        io.Fonts->AddFontFromFileTTF("assets/fonts/inconsolata/Inconsolata-Bold.ttf", fontSizeNormal, nullptr, iconRanges);
        io.Fonts->AddFontFromFileTTF("assets/fonts/inconsolata/Inconsolata-Bold.ttf", fontSizeNormal, nullptr, iconRanges);
        io.FontDefault = io.Fonts->Fonts[CX_FONT_REGULAR + CX_FONT_SIZE_NORMAL];

        // Set dark theme
        ImGui::StyleColorsDark();

        // Windows
        auto& colors = ImGui::GetStyle().Colors;
        colors[ImGuiCol_WindowBg] = ImVec4{ 0.1f, 0.105f, 0.11f, 1.0f };
        colors[ImGuiCol_PopupBg].w = 1.0f;

        // Headers
        colors[ImGuiCol_Header] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
        colors[ImGuiCol_HeaderHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
        colors[ImGuiCol_HeaderActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

        // Buttons
        colors[ImGuiCol_Button] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
        colors[ImGuiCol_ButtonHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
        colors[ImGuiCol_ButtonActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

        // Frame BG
        colors[ImGuiCol_FrameBg] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
        colors[ImGuiCol_FrameBgHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
        colors[ImGuiCol_FrameBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

        // Tabs
        colors[ImGuiCol_Tab] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
        colors[ImGuiCol_TabHovered] = ImVec4{ 0.38f, 0.3805f, 0.381f, 1.0f };
        colors[ImGuiCol_TabActive] = ImVec4{ 0.28f, 0.2805f, 0.281f, 1.0f };
        colors[ImGuiCol_TabUnfocused] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
        colors[ImGuiCol_TabUnfocusedActive] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };

        // Title
        colors[ImGuiCol_TitleBg] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
        colors[ImGuiCol_TitleBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
        colors[ImGuiCol_TitleBgCollapsed] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

        // Disable rounding if we are using multi viewports
        ImGuiStyle& style = ImGui::GetStyle();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            style.WindowRounding = 0.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        }
    }

}