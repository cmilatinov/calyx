#include "render/objects/Texture2D.h"
#include "render/Renderer.h"

#include "render/opengl/GLTexture2D.h"

namespace Calyx {

    Ref<Texture2D> Texture2D::Create(uint32 width, uint32 height, uint32 samples, TextureFormat format) {
        switch (Renderer::GetAPI()) {
            case RendererAPI::OPENGL:
                return CreateRef<GLTexture2D>(width, height, samples, format);
            default:
                CX_CORE_ASSERT(false, "Rendering API not supported!");
                return nullptr;
        }
    }

    Texture2D* Texture2D::Create(const String& file) {
        switch (Renderer::GetAPI()) {
            case RendererAPI::OPENGL:
                return new GLTexture2D(file);
            default:
                CX_CORE_ASSERT(false, "Rendering API not supported!");
                return nullptr;
        }
    }

}