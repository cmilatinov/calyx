#include "render/opengl/GLTexture2D.h"
#include "render/opengl/GLUtils.h"

namespace Calyx {

    GLTexture2D::GLTexture2D(uint32 width, uint32 height, uint32 samples, TextureFormat format)
        : m_width(width), m_height(height), m_samples(samples), m_format(format) {
        Init();
    }

    GLTexture2D::~GLTexture2D() {
        glDeleteTextures(1, &m_textureID);
    }

    void GLTexture2D::Init() {
        glGenTextures(1, &m_textureID);
        GLTexture2D::Bind();
        if (m_samples == 1) {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        }
        GLTexture2D::Resize(m_width, m_height);
    }

    void GLTexture2D::Bind() const {
        if (m_samples > 1)
            glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_textureID);
        else
            glBindTexture(GL_TEXTURE_2D, m_textureID);
    }

    void GLTexture2D::Unbind() const {
        if (m_samples > 1)
            glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
        else
            glBindTexture(GL_TEXTURE_2D, 0);
    }

    void GLTexture2D::AttachAsColor(uint32 attachmentIndex) const {
        glFramebufferTexture2D(GL_FRAMEBUFFER, GLColorAttachment(attachmentIndex), m_samples > 1 ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D, m_textureID, 0);
    }

    void GLTexture2D::AttachAsDepth() const {
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_samples > 1 ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D, m_textureID, 0);
    }

    void GLTexture2D::Resize(uint32 width, uint32 height) {
        GLTexture2D::Bind();
        m_width = width;
        m_height = height;
        if (m_samples > 1)
            glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, m_samples, GLInternalTextureFormat(m_format), m_width, m_height, GL_TRUE);
        else
            glTexImage2D(GL_TEXTURE_2D,0, GLInternalTextureFormat(m_format), m_width, m_height, 0, GLTextureFormat(m_format), GL_UNSIGNED_INT, nullptr);
    }

    void GLTexture2D::Load(const String& str) {
        // TODO: Load image data from file
    }

}