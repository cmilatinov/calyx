#pragma once

namespace Calyx {

    enum class ShaderDataType {
        Float = 0,
        Float2 = 1,
        Float3 = 2,
        Float4 = 3,
        Int = 4,
        Int2 = 5,
        Int3 = 6,
        Int4 = 7,
        Mat2 = 8,
        Mat3 = 9,
        Mat4 = 10,
        Bool = 11,
        None
    };

    static const uint32 s_shaderTypeSizes[] = {
        4, 8, 12, 16,
        4, 8, 12, 16,
        16, 36, 64,
        1
    };

    static const uint32 s_shaderTypeComponentCounts[] = {
        1, 2, 3, 4,
        1, 2, 3, 4,
        2, 3, 4,
        1
    };

    inline static uint32 ShaderDataTypeSize(ShaderDataType type) {
        CX_CORE_ASSERT(static_cast<int>(type) >= 0 && type < ShaderDataType::None, "Unknown shader type!");
        return s_shaderTypeSizes[static_cast<int>(type)];
    }

    inline static uint32 ShaderDataTypeComponentCount(ShaderDataType type) {
        CX_CORE_ASSERT(static_cast<int>(type) >= 0 && type < ShaderDataType::None, "Unknown shader type!");
        return s_shaderTypeComponentCounts[static_cast<int>(type)];
    }

    struct BufferElement {
        ShaderDataType type;
        String name;
        bool normalized = false;
        uint32 offset = 0;

        uint32 GetSize() const {
            return ShaderDataTypeSize(type);
        }

        uint32 GetComponentCount() const {
            return ShaderDataTypeComponentCount(type);
        }
    };

    class CALYX_API BufferLayout {

    public:
        BufferLayout() = default;

        BufferLayout(std::initializer_list<BufferElement> elements)
            : m_elements(elements) {
            CalculateOffsets();
        }

        uint32 GetStride() const { return m_stride; }
        const List<BufferElement>& GetElements() const { return m_elements; };

        List<BufferElement>::iterator begin() { return m_elements.begin(); }
        List<BufferElement>::iterator end() { return m_elements.end(); }
        List<BufferElement>::const_iterator begin() const { return m_elements.begin(); }
        List<BufferElement>::const_iterator end() const { return m_elements.end(); }

    private:
        void CalculateOffsets() {
            uint32 offset = 0;
            for (auto& element: m_elements) {
                element.offset = offset;
                uint32 elementSize = element.GetSize();
                offset += elementSize;
                m_stride += elementSize;
            }
        }

    private:
        List<BufferElement> m_elements;
        uint32 m_stride = 0;

    };

    class CALYX_API VertexBuffer {

    public:
        virtual ~VertexBuffer() = default;

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        virtual void SetData(uint32 size, const void* data) = 0;

        virtual const BufferLayout& GetLayout() const = 0;
        virtual void SetLayout(const BufferLayout& layout) = 0;

        static Ref<VertexBuffer> Create(uint32 size);
        static Ref<VertexBuffer> Create(uint32 size, const void* data);

    };

    class CALYX_API IndexBuffer {

    public:
        virtual ~IndexBuffer() = default;

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        virtual uint32 GetCount() const = 0;

        virtual void SetData(uint32 count, const uint32* indices) = 0;

        static Ref<IndexBuffer> Create(uint32 count, const uint32* indices);

    };

}