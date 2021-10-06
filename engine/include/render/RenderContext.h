#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "render/Renderer.h"

namespace Calyx {

    class CALYX_API RenderContext {
    public:
        virtual ~RenderContext() = default;

        virtual void Init() = 0;
        virtual void SwapBuffers() = 0;

        static Scope<RenderContext> Create(void* window);
    };

}