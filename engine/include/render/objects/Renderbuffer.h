#pragma once

#include "render/objects/IRenderTarget.h"

namespace Calyx {

    class Renderbuffer : public IRenderTarget {

    public:
        static Ref<Renderbuffer> Create(uint32 width, uint32 height, uint32 samples = 1, TextureFormat format = TextureFormat::RGBA8);

    };

}