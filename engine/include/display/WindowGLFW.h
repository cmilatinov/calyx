#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "display/Window.h"
#include "events/EventKeyboard.h"
#include "events/EventMouse.h"
#include "events/EventWindow.h"
#include "render/RenderContext.h"

namespace Calyx {

    /**
     * Implements a window object using GLFW as a backing library.
     */
    class CALYX_API WindowGLFW : public Window {

    public:
        explicit WindowGLFW(const WindowMode& mode);
        ~WindowGLFW() override;

        // Update
        void OnUpdate() override;

        // Getters
        uvec2 GetCenter() const override;
        uvec2 GetSize() const override;
        uint32 GetWidth() const override;
        uint32 GetHeight() const override;
        void* GetNativeWindow() const override;

        // Window callbacks
        static void OnWindowClose(GLFWwindow* window);
        static void OnWindowMove(GLFWwindow* window, int xpos, int ypos);
        static void OnWindowResize(GLFWwindow* window, int width, int height);
        static void OnWindowFocus(GLFWwindow* window, int focused);
        static void OnWindowMinimize(GLFWwindow* window, int iconified);
        static void OnWindowMaximize(GLFWwindow* window, int maximized);

        // Input callbacks
        static void OnWindowKey(GLFWwindow* window, int key, int scancode, int action, int mods);
        static void OnWindowMouseMove(GLFWwindow* window, double xpos, double ypos);
        static void OnWindowMouseButton(GLFWwindow* window, int button, int action, int mods);
        static void OnWindowMouseScroll(GLFWwindow* window, double xoffset, double yoffset);

    private:
        String m_title;
        WindowMode m_windowMode;
        GLFWwindow* m_windowHandle = nullptr;
        Scope<RenderContext> m_renderContext;

        static uint32 s_windowCount;

        void Init();
        void Destroy();
        void SetContextWindowHints();
        inline void DispatchEvent(Event& event);

        static int GetGLFWCursorMode(CursorMode cursorMode);
        static inline WindowGLFW* GetWindow(GLFWwindow* window);

    };

}