#include "render/objects/Buffer.h"


#include "render/Renderer.h"
#include "render/opengl/GLBuffer.h"

namespace Calyx {

    Ref<VertexBuffer> VertexBuffer::Create(uint32 size) {
        switch (Renderer::GetAPI()) {
            case RendererAPI::OPENGL:
                return CreateRef<GLVertexBuffer>(size);
        }
        CX_CORE_ASSERT(false, "Rendering API not supported!");
        return nullptr;
    }

    Ref<VertexBuffer> VertexBuffer::Create(uint32 size, const void* data) {
        switch (Renderer::GetAPI()) {
            case RendererAPI::OPENGL:
                return CreateRef<GLVertexBuffer>(size, data);
        }
        CX_CORE_ASSERT(false, "Rendering API not supported!");
        return nullptr;
    }

    Ref<IndexBuffer> IndexBuffer::Create(uint32 count, const uint32* indices) {
        switch (Renderer::GetAPI()) {
            case RendererAPI::OPENGL:
                return CreateRef<GLIndexBuffer>(count, indices);
        }
        CX_CORE_ASSERT(false, "Rendering API not supported!");
        return nullptr;
    }

}