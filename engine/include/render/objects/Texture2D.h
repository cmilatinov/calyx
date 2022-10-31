#pragma once

#include "render/objects/IRenderTarget.h"

namespace Calyx {

    class CALYX_API Texture2D : public IRenderTarget {

    public:
        virtual void Load(const String& file) = 0;

        static Ref<Texture2D>
        Create(uint32 width, uint32 height, uint32 samples = 1, TextureFormat format = TextureFormat::RGBA8);

    };

}