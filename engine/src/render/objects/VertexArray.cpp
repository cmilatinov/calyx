#include "render/objects/VertexArray.h"

#include "render/Renderer.h"
#include "render/opengl/GLVertexArray.h"

namespace Calyx {

    Ref<VertexArray> VertexArray::Create() {
        switch (Renderer::GetAPI()) {
            case RendererAPI::OPENGL:
                return CreateRef<GLVertexArray>();
        }
        CX_CORE_ASSERT(false, "Rendering API not supported!");
        return nullptr;
    }

}