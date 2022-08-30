#include "render/opengl/GLFramebuffer.h"
#include "render/opengl/GLUtils.h"

#include "core/Application.h"

namespace Calyx {

    GLFramebuffer::GLFramebuffer(const Framebuffer::Specification& spec)
        : m_spec(spec) {
        glGenFramebuffers(1, &m_framebufferID);
        Init();
    }

    GLFramebuffer::~GLFramebuffer() {
        glDeleteFramebuffers(1, &m_framebufferID);
    }

    void GLFramebuffer::Bind() const {
        glBindFramebuffer(GL_FRAMEBUFFER, m_framebufferID);
        glViewport(0, 0, m_spec.width, m_spec.height);
    }

    void GLFramebuffer::Unbind() const {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(0, 0, Window::GetMainWindow().GetWidth(), Window::GetMainWindow().GetHeight());
    }

    void GLFramebuffer::Resize(uint32_t width, uint32_t height) {
        m_spec.width = width;
        m_spec.height = height;
        Invalidate();
    }

    void GLFramebuffer::ClearColorAttachment(uint32_t attachmentIndex, uint32 value) {

    }

    void GLFramebuffer::ClearDepthAttachment(uint32 value) {

    }

    void GLFramebuffer::Blit(const Ref<Framebuffer>& other, uint32 srcAttachment, uint32 dstAttachment) {
        Bind();
        glBindFramebuffer(GL_READ_FRAMEBUFFER, m_framebufferID);
        glReadBuffer(GL_COLOR_ATTACHMENT0 + srcAttachment);
        ivec2 targetSize(0, 0);
        if (other == nullptr) {
            glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
            targetSize.x = Window::GetMainWindow().GetWidth();
            targetSize.y = Window::GetMainWindow().GetHeight();
            glDrawBuffer(GL_BACK);
        } else {
            glBindFramebuffer(GL_DRAW_FRAMEBUFFER, other->GetRendererID());
            targetSize.x = other->GetSpecification().width;
            targetSize.y = other->GetSpecification().height;
            glDrawBuffer(GL_COLOR_ATTACHMENT0 + dstAttachment);
        }
        glBlitFramebuffer(0, 0, m_spec.width, m_spec.height, 0, 0, targetSize.x, targetSize.y, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT, GL_NEAREST);
    }

    const IRenderTarget& GLFramebuffer::GetColorAttachment(uint32 attachmentIndex) const {
        return *m_colorAttachments[attachmentIndex];
    }

    const IRenderTarget& GLFramebuffer::GetDepthAttachment() const {
        return *m_depthAttachment;
    }

    void GLFramebuffer::Init() {
        GLFramebuffer::Bind();
        for (const auto& attachment : m_spec.attachments) {

            // Render target (can be a renderbuffer or texture)
            Ref<IRenderTarget> renderTarget = nullptr;

            // Check if it's a depth attachment
            bool isDepthAttachment = IsDepthFormat(attachment.format);

            // Use existing render target if given
            if (attachment.target != nullptr) {
                renderTarget = attachment.target;

            // Otherwise, create new render target
            } else {
                switch (attachment.type) {
                    case IRenderTarget::Type::RENDERBUFFER: {
                        renderTarget = Renderbuffer::Create(m_spec.width, m_spec.height, m_spec.samples, attachment.format);
                        break;
                    }
                    case IRenderTarget::Type::TEXTURE: {
                        renderTarget = Texture2D::Create(m_spec.width, m_spec.height, m_spec.samples, attachment.format);
                        break;
                    }
                    default: {
                        CX_CORE_ASSERT(false, "Invalid m_framebuffer attachment type!");
                        break;
                    }
                }
            }

            // Add attachment to m_framebuffer
            if (isDepthAttachment) {
                CX_CORE_ASSERT(m_depthAttachment == nullptr, "Framebuffer specification contains more than one depth attachment!");
                renderTarget->Bind();
                renderTarget->AttachAsDepth();
                m_depthAttachment = renderTarget;
            } else {
                CX_CORE_ASSERT(m_colorAttachments.size() < s_maxColorAttachments, "Framebuffer specification contains too many color attachments!");
                renderTarget->AttachAsColor(m_colorAttachments.size());
                m_colorAttachments.push_back(renderTarget);
            }
        }
        GLFramebuffer::Invalidate();
    }

    void GLFramebuffer::Invalidate() {
        if (m_depthAttachment != nullptr) {
            CX_CORE_ASSERT(m_depthAttachment->GetSamples() == m_spec.samples, "Framebuffer attachments must all have the same sample count!");
            m_depthAttachment->Resize(m_spec.width, m_spec.height);
        }

        for (auto& attachment : m_colorAttachments) {
            CX_CORE_ASSERT(attachment->GetSamples() == m_spec.samples, "Framebuffer attachments must all have the same sample count!");
            attachment->Resize(m_spec.width, m_spec.height);
        }
    }

    bool GLFramebuffer::IsComplete() const {
        Bind();
        return glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
    }

}