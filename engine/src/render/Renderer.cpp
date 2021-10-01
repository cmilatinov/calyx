#include "render/Renderer.h"

namespace Calyx {

    void Renderer::Init() {
        s_renderer = Create();
    }

    Scope<Renderer> Renderer::Create() {
        switch (Renderer::GetAPI()) {
            case OPENGL:
                return CreateScope<Renderer>();
            case NONE:
                break;
            case DIRECTX:
                break;
            case VULKAN:
                break;
        }
    }

}
