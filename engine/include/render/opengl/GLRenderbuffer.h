#pragma once

#include <glad/glad.h>

#include "render/objects/Renderbuffer.h"

namespace Calyx {

    class GLRenderbuffer : public Renderbuffer {

    public:
        GLRenderbuffer(uint32 width, uint32 height, uint32 samples, TextureFormat format);
        ~GLRenderbuffer() override;

        void Bind() const override;
        void Unbind() const override;

        void AttachAsColor(uint32 attachmentIndex) const override;
        void AttachAsDepth() const override;
        void Resize(uint32 width, uint32 height) override;

        uint32 GetRendererID() const override { return m_renderbufferID; }
        uint32 GetWidth() const override { return m_width; }
        uint32 GetHeight() const override { return m_height; }
        uint32 GetSamples() const override { return m_samples; }

        Type GetType() const override { return Type::RENDERBUFFER; }
        TextureFormat GetTextureFormat() const override { return m_format; }

    private:
        uint32 m_renderbufferID;
        uint32 m_width, m_height;
        uint32 m_samples;
        TextureFormat m_format;

    };

}