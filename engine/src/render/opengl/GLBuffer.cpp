#include "render/opengl/GLBuffer.h"

namespace Calyx {

    GLVertexBuffer::GLVertexBuffer(uint32 size) {
        glGenBuffers(1, &m_bufferID);
        CX_CORE_ASSERT(m_bufferID != 0, "Failed to create vertex buffer!");

        glBindBuffer(GL_ARRAY_BUFFER, m_bufferID);
        glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    GLVertexBuffer::GLVertexBuffer(uint32 size, const void* data) {
        glGenBuffers(1, &m_bufferID);
        CX_CORE_ASSERT(m_bufferID != 0, "Failed to create vertex buffer!");

        glBindBuffer(GL_ARRAY_BUFFER, m_bufferID);
        glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
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

    void GLVertexBuffer::SetData(uint32 size, const void* data) {
        glBindBuffer(GL_ARRAY_BUFFER, m_bufferID);
        glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
    }


    GLIndexBuffer::GLIndexBuffer(uint32 count, const uint32* indices)
        : m_count(count) {
        glGenBuffers(1, &m_bufferID);
        CX_CORE_ASSERT(m_bufferID != 0, "Failed to create index buffer!");

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_bufferID);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_count * sizeof(uint32), indices, GL_STATIC_DRAW);
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

    void GLIndexBuffer::SetData(uint32 count, const uint32* indices) {
        m_count = count;
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_bufferID);
        glBufferData(GL_ARRAY_BUFFER, m_count * sizeof(uint32), indices, GL_STATIC_DRAW);
    }

}