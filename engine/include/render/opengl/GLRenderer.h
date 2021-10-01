#pragma once

#include "render/Renderer.h"

namespace Calyx {

    class GLRenderer : public Renderer {

    public:
        void Initialize() override;
        void SetViewport(uint32 x, uint32 y, uint32 width, uint32 height) override;
        void SetClearColor(const vec4& color) override;
        void Clear() override;

    };

}