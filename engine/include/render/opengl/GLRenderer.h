#pragma once

#include <glad/glad.h>

#include "render/Renderer.h"

namespace Calyx {

    class CALYX_API GLRenderer : public Renderer {

    public:
        void Initialize() override;
        void SetViewport(uint32 x, uint32 y, uint32 width, uint32 height) override;
        void SetClearColor(const vec4& color) override;
        void Clear() override;

        void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32 indexCount) override;

    };

}