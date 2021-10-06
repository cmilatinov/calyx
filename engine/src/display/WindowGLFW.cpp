#include "display/WindowGLFW.h"

namespace Calyx {

    int32 WindowGLFW::s_windowCount = 0;

    WindowGLFW::Window * Window::Create(const WindowMode& mode) {
        return new WindowGLFW(mode);
    }

    WindowGLFW::WindowGLFW(const WindowMode& mode) : m_windowMode(mode) {
        Init();
    }

    WindowGLFW::~WindowGLFW() {
        Destroy();
    }

    void WindowGLFW::OnUpdate() {
        glfwPollEvents();
        m_renderContext->SwapBuffers();
    }

    uvec2 WindowGLFW::GetCenter() const {
        return uvec2(m_windowMode.width / 2, m_windowMode.height / 2);
    }

    uvec2 WindowGLFW::GetSize() const {
        return uvec2(m_windowMode.width, m_windowMode.height);
    }

    uint32 WindowGLFW::GetWidth() const {
        return m_windowMode.width;
    }

    uint32 WindowGLFW::GetHeight() const {
        return m_windowMode.height;
    }

    void* WindowGLFW::GetNativeWindow() const {
        return m_windowHandle;
    }

    void WindowGLFW::Init() {
        // GLFW initialization
        int success = glfwInit();
        CX_CORE_ASSERT(success, "Failed to initialize GLFW!");

        // Window hints
        glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
        glfwWindowHint(GLFW_DECORATED, m_windowMode.decorated ? GLFW_TRUE : GLFW_FALSE);
        glfwWindowHint(GLFW_FLOATING, m_windowMode.alwaysOnTop ? GLFW_TRUE : GLFW_FALSE);
        glfwWindowHint(GLFW_RESIZABLE, m_windowMode.resizable ? GLFW_TRUE : GLFW_FALSE);
        SetContextWindowHints();

        // Get video mode
        // TODO Exit app on null window handle or vmode
        const GLFWvidmode* vmode = glfwGetVideoMode(glfwGetPrimaryMonitor());
        CX_CORE_ASSERT(vmode != nullptr, "Failed to retrieve primary monitor video mode!");

        // Create window
        m_windowMode.width = m_windowMode.width == CX_DISPLAY_MAX_SIZE ? vmode->width : m_windowMode.width;
        m_windowMode.height = m_windowMode.height == CX_DISPLAY_MAX_SIZE ? vmode->height : m_windowMode.height;
        m_windowMode.x = m_windowMode.x == CX_DISPLAY_CENTER ? (vmode->width / 2) - (m_windowMode.width / 2) : m_windowMode.x;
        m_windowMode.y = m_windowMode.y == CX_DISPLAY_CENTER ? (vmode->height / 2) - (m_windowMode.height / 2) : m_windowMode.y;
        m_windowHandle = glfwCreateWindow(m_windowMode.width, m_windowMode.height, m_title.c_str(), nullptr, nullptr);
        CX_CORE_ASSERT(m_windowHandle != nullptr, "Failed to create GLFW window!");

        // Set fullscreen or position
        if (m_windowMode.fullscreen)
            glfwSetWindowMonitor(m_windowHandle, glfwGetPrimaryMonitor(), m_windowMode.x, m_windowMode.y, m_windowMode.width, m_windowMode.height, GLFW_DONT_CARE);
        else
            glfwSetWindowPos(m_windowHandle, m_windowMode.x, m_windowMode.y);

        // Input mode + vsync + window pointer
        glfwSetInputMode(m_windowHandle, GLFW_CURSOR, GetGLFWCursorMode(m_windowMode.cursorMode));
        glfwSwapInterval(m_windowMode.vsync);
        glfwSetWindowUserPointer(m_windowHandle, this);

        // Window callbacks
        glfwSetWindowCloseCallback(m_windowHandle, &OnWindowClose);
        glfwSetWindowPosCallback(m_windowHandle, &OnWindowMove);
        glfwSetWindowSizeCallback(m_windowHandle, &OnWindowResize);
        glfwSetWindowFocusCallback(m_windowHandle, &OnWindowFocus);
        glfwSetWindowIconifyCallback(m_windowHandle, &OnWindowMinimize);
        glfwSetWindowMaximizeCallback(m_windowHandle, &OnWindowMaximize);

        // Input callbacks
        glfwSetKeyCallback(m_windowHandle, &OnWindowKey);
        glfwSetCursorPosCallback(m_windowHandle, &OnWindowMouseMove);
        glfwSetMouseButtonCallback(m_windowHandle, &OnWindowMouseButton);
        glfwSetScrollCallback(m_windowHandle, &OnWindowMouseScroll);

        // Show window
        glfwShowWindow(m_windowHandle);
        s_windowCount++;

        // Create context
        glfwMakeContextCurrent(m_windowHandle);
        m_renderContext = RenderContext::Create(m_windowHandle);
        m_renderContext->Init();

    }

    void WindowGLFW::Destroy() {
        glfwDestroyWindow(m_windowHandle);
        if (--s_windowCount <= 0)
            glfwTerminate();
    }

    inline void WindowGLFW::DispatchEvent(Event& event) {
        if (m_eventCallback == nullptr)
            return;
        m_eventCallback(event);
    }

    int WindowGLFW::GetGLFWCursorMode(CursorMode cursorMode) {
        switch (cursorMode) {
            case NORMAL:
                return GLFW_CURSOR_NORMAL;
            case HIDDEN:
                return GLFW_CURSOR_HIDDEN;
            case DISABLED:
                return GLFW_CURSOR_DISABLED;
            default:
                return -1;
        }
    }

    inline WindowGLFW* WindowGLFW::GetWindow(GLFWwindow* window) {
        return static_cast<WindowGLFW*>(glfwGetWindowUserPointer(window));
    }

    void WindowGLFW::OnWindowClose(GLFWwindow* window) {
        EventWindowClose event;
        GetWindow(window)->DispatchEvent(event);
    }

    void WindowGLFW::OnWindowMove(GLFWwindow* window, int xpos, int ypos) {
        EventWindowMove event(xpos, ypos);
        GetWindow(window)->DispatchEvent(event);
    }

    void WindowGLFW::OnWindowResize(GLFWwindow* window, int width, int height) {
        EventWindowResize event(width, height);
        GetWindow(window)->DispatchEvent(event);
    }

    void WindowGLFW::OnWindowFocus(GLFWwindow* window, int focused) {
        switch (focused) {
            case GLFW_TRUE: {
                EventWindowFocus event;
                GetWindow(window)->DispatchEvent(event);
                return;
            }
            case GLFW_FALSE: {
                EventWindowLostFocus event;
                GetWindow(window)->DispatchEvent(event);
                return;
            }
            default:
                return;
        }
    }

    void WindowGLFW::OnWindowMinimize(GLFWwindow* window, int iconified) {
        if (iconified == GLFW_FALSE) {
            EventWindowRestore event;
            GetWindow(window)->DispatchEvent(event);
            return;
        }

        EventWindowMinimize event;
        GetWindow(window)->DispatchEvent(event);
    }

    void WindowGLFW::OnWindowMaximize(GLFWwindow* window, int maximized) {
        if (maximized == GLFW_FALSE) {
            EventWindowRestore event;
            GetWindow(window)->DispatchEvent(event);
            return;
        }

        EventWindowMaximize event;
        GetWindow(window)->DispatchEvent(event);
    }

    void WindowGLFW::OnWindowKey(GLFWwindow* window, int key, int scancode, int action, int mods) {
        switch (action) {
            case GLFW_PRESS: {
                EventKeyPress event(key);
                GetWindow(window)->DispatchEvent(event);
                return;
            }
            case GLFW_RELEASE: {
                EventKeyRelease event(key);
                GetWindow(window)->DispatchEvent(event);
                return;
            }
            case GLFW_REPEAT: {
                EventKeyPress event(key, true);
                GetWindow(window)->DispatchEvent(event);
                return;
            }
            default:
                return;
        }
    }

    void WindowGLFW::OnWindowMouseMove(GLFWwindow* window, double xpos, double ypos) {
        EventMouseMove event(static_cast<float>(xpos), static_cast<float>(ypos));
        GetWindow(window)->DispatchEvent(event);
    }

    void WindowGLFW::OnWindowMouseButton(GLFWwindow* window, int button, int action, int mods) {
        switch (action) {
            case GLFW_PRESS: {
                EventMouseButtonPress event(button);
                GetWindow(window)->DispatchEvent(event);
                return;
            }
            case GLFW_RELEASE: {
                EventMouseButtonRelease event(button);
                GetWindow(window)->DispatchEvent(event);
                return;
            }
            default:
                return;
        }
    }

    void WindowGLFW::OnWindowMouseScroll(GLFWwindow* window, double xoffset, double yoffset) {
        EventMouseScroll event(static_cast<float>(xoffset), static_cast<float>(yoffset));
        GetWindow(window)->DispatchEvent(event);
    }

    void WindowGLFW::SetContextWindowHints() {
        switch (Renderer::GetAPI()) {
            case RendererAPI::OPENGL:
                glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
                glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
                return;
        }
    }

}