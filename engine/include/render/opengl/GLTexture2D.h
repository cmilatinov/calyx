#pragma once

#include <glad/glad.h>

#include "render/objects/Texture2D.h"

namespace Calyx {

    class CALYX_API GLTexture2D : public Texture2D {

    public:
        explicit GLTexture2D(const String& file);
        GLTexture2D(uint32 width, uint32 height, uint32 m_samples, TextureFormat format);
        ~GLTexture2D() override;

        void Bind() const override;
        void Unbind() const override;

        void AttachAsColor(uint32 attachmentIndex) const override;
        void AttachAsDepth() const override;
        void Resize(uint32 width, uint32 height) override;

        uint32 GetRendererID() const override { return m_textureID; }
        uint32 GetWidth() const override { return m_width; }
        uint32 GetHeight() const override { return m_height; }
        uint32 GetSamples() const override { return m_samples; }

        Type GetType() const override { return Type::TEXTURE; }
        TextureFormat GetTextureFormat() const override { return m_format; }

    private:
        uint32 m_textureID = 0;
        uint32 m_width = 0, m_height = 0;
        uint32 m_samples = 1;
        TextureFormat m_format = TextureFormat::RGBA8;

        void Init(uint8* data = nullptr);
        void Destroy();

        bool Load(const String& file);
        void Resize(uint32 width, uint32 height, uint8* data);

    };

}