#include "render/objects/Query.h"
#include "render/Renderer.h"
#include "render/opengl/GLQuery.h"

namespace Calyx {

    Ref<Query> Query::Create() {
        switch (Renderer::GetAPI()) {
            case RendererAPI::OPENGL:
                return CreateRef<GLQuery>();
            default:
                CX_CORE_ASSERT(false, "Rendering API not supported!");
                return nullptr;
        }
    }

}