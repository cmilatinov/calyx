#include "render/opengl/GLRenderContext.h"

namespace Calyx {

    GLRenderContext::GLRenderContext(GLFWwindow* window)
        : m_windowHandle(window) {}

    void GLRenderContext::Init() {
        CX_CORE_INFO("Initializing OpenGL context ...");

        int status = gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));
        CX_CORE_ASSERT(status, "Failed to initialize Glad!");

        auto* vendor = reinterpret_cast<const char*>(glGetString(GL_VENDOR));
        auto* renderer = reinterpret_cast<const char*>(glGetString(GL_RENDERER));
        auto* version = reinterpret_cast<const char*>(glGetString(GL_VERSION));

        CX_CORE_INFO("");
        CX_CORE_INFO("OpenGL Info:");
        CX_CORE_INFO("    Vendor:      {0}", vendor);
        CX_CORE_INFO("    Renderer:    {0}", renderer);
        CX_CORE_INFO("    Version:     {0}", version);
        int index = 0;
        const char* extension = nullptr;
        do {
            extension = reinterpret_cast<const char*>(glGetStringi(GL_EXTENSIONS, index));
            index++;
            if (extension != nullptr) {
                CX_CORE_INFO("    Extension:   {0}", extension);
            }
        } while (extension != nullptr);
        CX_CORE_INFO("");

        CX_CORE_ASSERT(GLVersion.major >= 4, "Calyx requires at least OpenGL version 4.0!");
        glDebugMessageCallback(GLRenderContext::DebugMessageCallback, nullptr);
    }

    void GLRenderContext::SwapBuffers() {
        glfwSwapBuffers(m_windowHandle);
    }

    void GLRenderContext::DebugMessageCallback(
        GLenum source, GLenum type, GLuint id,
        GLenum severity, GLsizei length, const GLchar* message,
        const void* userParam
    ) {
        if (severity == GL_DEBUG_SEVERITY_NOTIFICATION) return;
        CX_CORE_ERROR("{} {}", severity, message);
    }

}
