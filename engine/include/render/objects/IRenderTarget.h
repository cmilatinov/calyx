#pragma once

#include "render/objects/Texture.h"

namespace Calyx {

    class CALYX_API IRenderTarget {

    public:
        enum class Type {
            RENDERBUFFER,
            TEXTURE
        };

    public:
        virtual ~IRenderTarget() = default;

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        virtual void AttachAsColor(uint32 attachmentIndex) const = 0;
        virtual void AttachAsDepth() const = 0;
        virtual void Resize(uint32 width, uint32 height) = 0;

        virtual uint32 GetRendererID() const = 0;
        virtual uint32 GetWidth() const = 0;
        virtual uint32 GetHeight() const = 0;
        virtual uint32 GetSamples() const = 0;

        virtual Type GetType() const = 0;
        virtual TextureFormat GetTextureFormat() const = 0;

    };

}