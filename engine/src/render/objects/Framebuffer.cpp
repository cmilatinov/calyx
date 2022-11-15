#include "render/objects/Framebuffer.h"
#include "render/Renderer.h"

#include "render/opengl/GLFramebuffer.h"

namespace Calyx {

    /// Creates ands binds a new Framebuffer object according to the given specification.
    /// \param spec - The Framebuffer specification to use when creating the Framebuffer.
    /// \return The newly created Framebuffer object.
    Ref<Framebuffer> Framebuffer::Create(const Framebuffer::Specification& spec) {
        switch (Renderer::GetAPI()) {
            case RendererAPI::OPENGL:
                return CreateRef<GLFramebuffer>(spec);
        }
        CX_CORE_ASSERT(false, "Rendering API not supported!");
        return nullptr;
    }

}