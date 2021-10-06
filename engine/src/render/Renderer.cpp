#include "render/opengl/GLRenderer.h"

namespace Calyx {

    RendererAPI Renderer::s_api = OPENGL;
    Scope<Renderer> Renderer::s_renderer = nullptr;

    void Renderer::Init() {
        s_renderer = Create();
    }

    Scope<Renderer> Renderer::Create() {
        switch (Renderer::GetAPI()) {
            case OPENGL:
                return CreateScope<GLRenderer>();
        }
        CX_CORE_ASSERT(false, "Renderer not supported!");
        return nullptr;
    }

}
