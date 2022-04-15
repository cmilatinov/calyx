#pragma once

#include <glad/glad.h>

#include "render/objects/IRenderTarget.h"
#include "render/objects/Framebuffer.h"
#include "render/objects/Renderbuffer.h"
#include "render/objects/Texture2D.h"

namespace Calyx {

    class GLFramebuffer : public Framebuffer {

    public:
        static const uint32 s_maxColorAttachments = 4;

    public:
        GLFramebuffer(const Framebuffer::Specification& spec);
        ~GLFramebuffer() override;

        void Bind() const override;
        void Unbind() const override;

        void Resize(uint32_t width, uint32_t height) override;
        void ClearColorAttachment(uint32_t attachmentIndex, uint32 value) override;
        void ClearDepthAttachment(uint32 value) override;
        void Blit(const Ref<Framebuffer>& other, uint32 srcAttachment, uint32 dstAttachment) override;

        const IRenderTarget& GetColorAttachment(uint32 attachmentIndex) const override;
        const IRenderTarget& GetDepthAttachment() const override;

        const Framebuffer::Specification& GetSpecification() const override { return m_spec; }
        uint32 GetRendererID() const override { return m_framebufferID; }
        bool IsComplete() const override;

    private:
        Framebuffer::Specification m_spec;
        uint32 m_framebufferID;
        Ref<IRenderTarget> m_depthAttachment;
        List<Ref<IRenderTarget>> m_colorAttachments;

        void Init();
        void Invalidate();

    };


}