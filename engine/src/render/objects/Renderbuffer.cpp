#include "render/objects/Renderbuffer.h"
#include "render/Renderer.h"

#include "render/opengl/GLRenderbuffer.h"

namespace Calyx {

    Ref<Renderbuffer> Renderbuffer::Create(uint32 width, uint32 height, uint32 samples, TextureFormat format) {
        switch (Renderer::GetAPI()) {
            case RendererAPI::OPENGL:
                return CreateRef<GLRenderbuffer>(width, height, samples, format);
        }
        CX_CORE_ASSERT(false, "Rendering API not supported!");
        return nullptr;
    }

}