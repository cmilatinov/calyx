#pragma once

#include <glad/glad.h>

#include "render/objects/Buffer.h"

namespace Calyx {

    class GLVertexBuffer : public VertexBuffer {

    public:
        GLVertexBuffer(uint32 size);
        GLVertexBuffer(uint32 size, const float* vertices);
        ~GLVertexBuffer() override;

        void Bind() const override;
        void Unbind() const override;

        void SetData(const void* data, uint32 size) override;

        const BufferLayout& GetLayout() const override { return m_bufferLayout; };
        void SetLayout(const BufferLayout& layout) override { m_bufferLayout = layout; };

    private:
        BufferLayout m_bufferLayout;
        uint32 m_bufferID;

    };

    class GLIndexBuffer : public IndexBuffer {

    public:
        GLIndexBuffer(uint32 count, const uint32* indices);
        virtual ~GLIndexBuffer();

        virtual void Bind() const;
        virtual void Unbind() const;

        virtual uint32 GetCount() const { return m_count; };

    private:
        uint32 m_bufferID;
        uint32 m_count;

    };

}

