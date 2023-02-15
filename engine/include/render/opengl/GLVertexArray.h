#pragma once

#include <glad/glad.h>

#include "render/objects/VertexArray.h"

namespace Calyx {

    class CALYX_API GLVertexArray : public VertexArray {

    public:
        GLVertexArray();
        ~GLVertexArray() override;

        void Bind() const override;
        void Unbind() const override;

        void AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) override;
        void SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer) override;

        const List<Ref<VertexBuffer>>& GetVertexBuffers() const override { return m_vertexBuffers; };
        const Ref<IndexBuffer>& GetIndexBuffer() const override { return m_indexBuffer; };

    private:
        uint32 m_vertexArrayID;
        uint32 m_nextVertexAttribIndex = 0;
        List<Ref<VertexBuffer>> m_vertexBuffers;
        Ref<IndexBuffer> m_indexBuffer;

    };

}