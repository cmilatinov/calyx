#pragma once

#include "render/objects/IRenderTarget.h"
#include "assets/AssetRegistry.h"

namespace Calyx {

    class CALYX_API Texture2D : public IRenderTarget, public Asset<Texture2D> {
        CX_ASSET_REGISTRY_FRIEND();

    public:

        static Ref<Texture2D> Create(
            uint32 width,
            uint32 height,
            uint32 samples = 1,
            TextureFormat format = TextureFormat::RGBA8
        );

    private:
        static Texture2D* Create(const String& file);

    };

}