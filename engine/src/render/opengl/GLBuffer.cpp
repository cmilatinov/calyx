#include "render/opengl/GLBuffer.h"

namespace Calyx {

    GLVertexBuffer::GLVertexBuffer(uint32 size) {
        glGenBuffers(1, &m_bufferID);
        CX_CORE_ASSERT(m_bufferID != 0, "Failed to create vertex buffer!");

        glBindBuffer(GL_ARRAY_BUFFER, m_bufferID);
        glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    GLVertexBuffer::GLVertexBuffer(uint32 size, const float* vertices) {
        glGenBuffers(1, &m_bufferID);
        CX_CORE_ASSERT(m_bufferID != 0, "Failed to create vertex buffer!");

        glBindBuffer(GL_ARRAY_BUFFER, m_bufferID);
        glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    GLVertexBuffer::~GLVertexBuffer() {
        glDeleteBuffers(1, &m_bufferID);
    }

    void GLVertexBuffer::Bind() const {
        glBindBuffer(GL_ARRAY_BUFFER, m_bufferID);
    }

    void GLVertexBuffer::Unbind() const {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void GLVertexBuffer::SetData(const void* data, uint32 size) {
        glBindBuffer(GL_ARRAY_BUFFER, m_bufferID);
        glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
    }


    GLIndexBuffer::GLIndexBuffer(uint32 count, const uint32* indices) {
        glGenBuffers(1, &m_bufferID);
        CX_CORE_ASSERT(m_bufferID != 0, "Failed to create index buffer!");

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_bufferID);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32), indices, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    GLIndexBuffer::~GLIndexBuffer() {
        glDeleteBuffers(1, &m_bufferID);
    }

    void GLIndexBuffer::Bind() const {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_bufferID);
    }

    void GLIndexBuffer::Unbind() const {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

}