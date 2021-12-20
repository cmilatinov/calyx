#include "render/objects/Framebuffer.h"
#include "render/Renderer.h"

#include "render/opengl/GLFramebuffer.h"

namespace Calyx {

    Ref<Framebuffer> Framebuffer::Create(const Framebuffer::Spec& spec) {
        switch (Renderer::GetAPI()) {
            case RendererAPI::OPENGL:
                return CreateRef<GLFramebuffer>(spec);
        }
        CX_CORE_ASSERT(false, "Rendering API not supported!");
        return nullptr;
    }

}