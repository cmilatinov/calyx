#pragma once

#include <glad/glad.h>

#include "render/objects/Buffer.h"

namespace Calyx {

    class CALYX_API GLVertexBuffer : public VertexBuffer {

    public:
        explicit GLVertexBuffer(uint32 size);
        GLVertexBuffer(uint32 size, const void* data);
        ~GLVertexBuffer() override;

        void Bind() const override;
        void Unbind() const override;

        void SetData(uint32 size, const void* data) override;

        const BufferLayout& GetLayout() const override { return m_bufferLayout; };
        void SetLayout(const BufferLayout& layout) override { m_bufferLayout = layout; };

    private:
        BufferLayout m_bufferLayout;
        uint32 m_bufferID;

    };

    class GLIndexBuffer : public IndexBuffer {

    public:
        GLIndexBuffer(uint32 count, const uint32* indices);
        ~GLIndexBuffer() override;

        void Bind() const override;
        void Unbind() const override;

        void SetData(uint32 count, const uint32* indices) override;

        uint32 GetCount() const override { return m_count; };

    private:
        uint32 m_bufferID;
        uint32 m_count;

    };

}

