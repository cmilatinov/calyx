#pragma once

#include "render/objects/Buffer.h"

namespace Calyx {

    static const uint32 s_shaderTypeAlignments[] = {
            4, 8, 16, 16,
            4, 8, 16, 16
    };

    class UniformBufferLayout {

    public:
        UniformBufferLayout() {}

        UniformBufferLayout(std::initializer_list<BufferElement> elements)
            : m_elements(elements) {
            CalculateOffsets();
        }

        const List<BufferElement>& GetElements() const { return m_elements; };

        List<BufferElement>::iterator begin() { return m_elements.begin(); }
        List<BufferElement>::iterator end() { return m_elements.end(); }
        List<BufferElement>::const_iterator begin() const { return m_elements.begin(); }
        List<BufferElement>::const_iterator end() const { return m_elements.end(); }

    private:
        void CalculateOffsets() {
            uint32 offset = 0;
            for (auto& element : m_elements) {
//                switch
            }
        }

    private:
        List<BufferElement> m_elements;

    };

    class UniformBuffer {

//    public:
//        virtual ~UniformBuffer() = default;
//
//        virtual void Bind() const = 0;
//        virtual void Unbind() const = 0;
//
//        virtual void SetData(const void* data, uint32 size) = 0;
//
//        virtual const BufferLayout& GetLayout() const = 0;
//        virtual void SetLayout(const BufferLayout& layout) = 0;
//
//    private:
//        uint32
//        BufferLayout m_bufferLayout;

    };

}