#include "render/opengl/GLRenderbuffer.h"
#include "render/opengl/GLUtils.h"

namespace Calyx {

    GLRenderbuffer::GLRenderbuffer(uint32 width, uint32 height, uint32 samples, TextureFormat format)
        : m_width(width), m_height(height), m_samples(samples), m_format(format) {
        glGenRenderbuffers(1, &m_renderbufferID);
        GLRenderbuffer::Resize(m_width, m_height);
    }

    GLRenderbuffer::~GLRenderbuffer() {
        glDeleteRenderbuffers(1, &m_renderbufferID);
    }

    void GLRenderbuffer::Bind() const {
        glBindRenderbuffer(GL_RENDERBUFFER, m_renderbufferID);
    }

    void GLRenderbuffer::Unbind() const {
        glBindRenderbuffer(GL_RENDERBUFFER, 0);
    }

    void GLRenderbuffer::AttachAsColor(uint32 attachmentIndex) const {
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GLColorAttachment(attachmentIndex), GL_RENDERBUFFER, m_renderbufferID);
    }

    void GLRenderbuffer::AttachAsDepth() const {
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_renderbufferID);
    }

    void GLRenderbuffer::Resize(uint32 width, uint32 height) {
        GLRenderbuffer::Bind();
        m_width = width;
        m_height = height;
        if (m_samples > 1)
            glRenderbufferStorageMultisample(GL_RENDERBUFFER, m_samples, GLInternalTextureFormat(m_format), m_width, m_height);
        else
            glRenderbufferStorage(GL_RENDERBUFFER, GLInternalTextureFormat(m_format), m_width, m_height);
    }

}