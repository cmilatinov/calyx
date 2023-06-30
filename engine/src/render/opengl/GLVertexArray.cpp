#include "render/opengl/GLVertexArray.h"

namespace Calyx {

    static const uint32 s_openglShaderTypes[] = {
        GL_FLOAT,
        GL_FLOAT,
        GL_FLOAT,
        GL_FLOAT,

        GL_INT,
        GL_INT,
        GL_INT,
        GL_INT,

        GL_FLOAT,
        GL_FLOAT,
        GL_FLOAT,

        GL_BOOL
    };

    inline static GLenum ShaderDataTypeToOpenGLType(ShaderDataType type) {
        CX_CORE_ASSERT(static_cast<int>(type) >= 0 && type < ShaderDataType::None, "Unknown shader type!");
        return s_openglShaderTypes[static_cast<int>(type)];
    }

    GLVertexArray::GLVertexArray() {
        glGenVertexArrays(1, &m_vertexArrayID);
        CX_CORE_ASSERT(m_vertexArrayID != 0, "Failed to create vertex array!");
    }

    GLVertexArray::~GLVertexArray() {
        glDeleteVertexArrays(1, &m_vertexArrayID);
    }

    void GLVertexArray::Bind() const {
        glBindVertexArray(m_vertexArrayID);
    }

    void GLVertexArray::Unbind() const {
        glBindVertexArray(0);
    }

    void GLVertexArray::AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) {
        CX_CORE_ASSERT(!vertexBuffer->GetLayout().GetElements().empty(), "Vertex buffer has no layout!");
        glBindVertexArray(m_vertexArrayID);
        vertexBuffer->Bind();

        auto layout = vertexBuffer->GetLayout();
        for (auto& element: layout) {
            switch (element.type) {
                case ShaderDataType::Float:
                case ShaderDataType::Float2:
                case ShaderDataType::Float3:
                case ShaderDataType::Float4: {
                    glEnableVertexAttribArray(m_nextVertexAttribIndex);
                    glVertexAttribPointer(
                        m_nextVertexAttribIndex,
                        element.GetComponentCount(),
                        ShaderDataTypeToOpenGLType(element.type),
                        element.normalized ? GL_TRUE : GL_FALSE,
                        layout.GetStride(),
                        const_cast<const void*>(reinterpret_cast<void*>(static_cast<uint64>(element.offset)))
                    );
                    m_nextVertexAttribIndex++;
                    break;
                }
                case ShaderDataType::Int:
                case ShaderDataType::Int2:
                case ShaderDataType::Int3:
                case ShaderDataType::Int4:
                case ShaderDataType::Bool: {
                    glEnableVertexAttribArray(m_nextVertexAttribIndex);
                    glVertexAttribIPointer(
                        m_nextVertexAttribIndex,
                        element.GetComponentCount(),
                        ShaderDataTypeToOpenGLType(element.type),
                        layout.GetStride(),
                        const_cast<const void*>(reinterpret_cast<void*>(static_cast<uint64>(element.offset)))
                    );
                    m_nextVertexAttribIndex++;
                    break;
                }
                case ShaderDataType::Mat2:
                case ShaderDataType::Mat3:
                case ShaderDataType::Mat4: {
                    uint32 count = element.GetComponentCount();
                    for (uint32 i = 0; i < count; i++) {
                        glEnableVertexAttribArray(m_nextVertexAttribIndex);
                        glVertexAttribPointer(
                            m_nextVertexAttribIndex,
                            count,
                            ShaderDataTypeToOpenGLType(element.type),
                            element.normalized ? GL_TRUE : GL_FALSE,
                            layout.GetStride(),
                            (const void*)(element.offset + sizeof(float) * count * i));
                        glVertexAttribDivisor(m_nextVertexAttribIndex, 1);
                        m_nextVertexAttribIndex++;
                    }
                    break;
                }
                default: {
                    CX_CORE_ASSERT(false, "Unknown shader type!");
                    break;
                }
            }
        }

        m_vertexBuffers.push_back(vertexBuffer);
    }

    void GLVertexArray::SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer) {
        glBindVertexArray(m_vertexArrayID);
        indexBuffer->Bind();
        m_indexBuffer = indexBuffer;
    }

    void GLVertexArray::SetVertexBufferEnabled(uint32 index, bool enabled) {
        glBindVertexArray(m_vertexArrayID);
        if (enabled) {
            glEnableVertexAttribArray(index);
        } else {
            glDisableVertexAttribArray(index);
        }
    }

}