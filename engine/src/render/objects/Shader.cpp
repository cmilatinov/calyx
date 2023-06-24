#include "render/objects/Shader.h"
#include "render/Renderer.h"

#include "render/opengl/GLShader.h"

namespace Calyx {

    Shader* Shader::Create(const String& file) {
        switch (Renderer::GetAPI()) {
            case RendererAPI::OPENGL:
                return new GLShader(file);
            default:
                CX_CORE_ASSERT(false, "Rendering API not supported!");
                return nullptr;
        }
    }

}