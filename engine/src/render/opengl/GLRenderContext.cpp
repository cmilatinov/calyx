#include "render/opengl/GLRenderContext.h"

namespace Calyx {

    GLRenderContext::GLRenderContext(GLFWwindow* window)
        : m_windowHandle(window) {}

    void GLRenderContext::Init() {
        CX_CORE_INFO("Initializing OpenGL context ...");

        glfwMakeContextCurrent(m_windowHandle);
        int status = gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));
        CX_CORE_ASSERT(status, "Failed to initialize Glad!");

        CX_CORE_INFO("");
        CX_CORE_INFO("OpenGL Info:");
        CX_CORE_INFO("    Vendor:      {0}", glGetString(GL_VENDOR));
        CX_CORE_INFO("    Renderer:    {0}", glGetString(GL_RENDERER));
        CX_CORE_INFO("    Version:     {0}", glGetString(GL_VERSION));
        CX_CORE_INFO("");

        CX_CORE_ASSERT(GLVersion.major >= 4, "Calyx requires at least OpenGL version 4.0!");
    }

    void GLRenderContext::SwapBuffers() {
        glfwSwapBuffers(m_windowHandle);
    }

}
