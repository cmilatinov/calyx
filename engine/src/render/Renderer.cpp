#include "render/opengl/GLRenderer.h"

namespace Calyx {

    RendererAPI Renderer::s_api = RendererAPI::OPENGL;
    Scope<Renderer> Renderer::s_renderer = nullptr;

    void Renderer::Init() {
        s_renderer = Create();
        s_renderer->Initialize();
    }

    Scope<Renderer> Renderer::Create() {
        switch (Renderer::GetAPI()) {
            case RendererAPI::OPENGL:
                return CreateScope<GLRenderer>();
        }
        CX_CORE_ASSERT(false, "Renderer not supported!");
        return nullptr;
    }

}
