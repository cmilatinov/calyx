#include "render/opengl/GLRenderer.h"

namespace Calyx {

    Renderer::API Renderer::s_api = OPENGL;

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
