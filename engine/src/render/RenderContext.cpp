#include "render/RenderContext.h"
#include "render/Renderer.h"

namespace Calyx {

    Scope<RenderContext> RenderContext::Create(void* window) {
        switch(Renderer::GetAPI()) {
            case Renderer::API::OPENGL:
                // TODO Create and return OpenGL render context
                break;
        }
        CX_CORE_ASSERT(false, "Unsupported rendering API!");
        return nullptr;
    }

}