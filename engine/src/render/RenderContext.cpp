#include "render/RenderContext.h"
#include "render/opengl/GLRenderContext.h"

namespace Calyx {

    Scope<RenderContext> RenderContext::Create(void* window) {
        switch (Renderer::GetAPI()) {
            case RendererAPI::OPENGL:
                return CreateScope<GLRenderContext>(static_cast<GLFWwindow*>(window));
        }
        CX_CORE_ASSERT(false, "Unsupported rendering API!");
        return nullptr;
    }

}