#pragma once

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include "render/RenderContext.h"

namespace Calyx {

    class CALYX_API GLRenderContext : public RenderContext {

    public:
        explicit GLRenderContext(GLFWwindow* window);

        void Init() override;
        void SwapBuffers() override;

    private:
        GLFWwindow* m_windowHandle;

    };

}