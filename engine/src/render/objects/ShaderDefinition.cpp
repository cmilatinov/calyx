#include "render/objects/ShaderDefinition.h"
#include "render/Renderer.h"

#include "render/opengl/GLShaderDefinition.h"

namespace Calyx {

    ShaderDefinition* ShaderDefinition::Create(const String& file) {
        switch (Renderer::GetAPI()) {
            case RendererAPI::OPENGL:
                return new GLShaderDefinition(file);
            default:
                CX_CORE_ASSERT(false, "Rendering API not supported!");
                return nullptr;
        }
    }

}