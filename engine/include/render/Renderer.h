#pragma once

#include "render/objects/VertexArray.h"

namespace Calyx {

    class Application;

    enum class RendererAPI {
        NONE,
        OPENGL,
        DIRECTX,
        VULKAN
    };

    enum class PrimitiveType {
        TRIANGLE,
        QUAD,
        POINT,
        LINE,
        LINE_LOOP
    };

    class CALYX_API Renderer {

        friend class ::Calyx::Application;

    public:
        virtual ~Renderer() = default;

        virtual void Initialize() = 0;
        virtual void SetViewport(uint32 x, uint32 y, uint32 width, uint32 height) = 0;
        virtual void SetClearColor(const vec4& color) = 0;
        virtual void Clear() = 0;

        virtual void Draw(
            const Ref<VertexArray>& vertexArray,
            uint32 vertexCount,
            PrimitiveType type = PrimitiveType::TRIANGLE
        ) = 0;

        virtual void DrawIndexed(
            const Ref<VertexArray>& vertexArray,
            uint32 indexCount,
            PrimitiveType type = PrimitiveType::TRIANGLE
        ) = 0;

        virtual void DrawInstanced(
            const Ref<VertexArray>& vertexArray,
            uint32 vertexCount,
            uint32 instanceCount,
            PrimitiveType type = PrimitiveType::TRIANGLE
        ) = 0;

        virtual void DrawIndexedInstanced(
            const Ref<VertexArray>& vertexArray,
            uint32 indexCount,
            uint32 instanceCount,
            PrimitiveType type = PrimitiveType::TRIANGLE
        ) = 0;

        static inline RendererAPI GetAPI() {
            return s_api;
        }

        static inline Renderer& GetInstance() {
            return *s_renderer;
        }

        static void Init();

    private:
        static RendererAPI s_api;
        static Scope<Renderer> s_renderer;

        static Scope<Renderer> Create();

    };

    namespace RenderCommand {

        static inline void SetViewport(uint32 x, uint32 y, uint32 width, uint32 height) {
            Renderer::GetInstance().SetViewport(x, y, width, height);
        }

        static inline void SetClearColor(const vec4& color) {
            Renderer::GetInstance().SetClearColor(color);
        }

        static inline void Clear() {
            Renderer::GetInstance().Clear();
        }

        static inline void Draw(
            const Ref<VertexArray>& vertexArray,
            uint32 vertexCount,
            PrimitiveType type = PrimitiveType::TRIANGLE
        ) {
            Renderer::GetInstance().Draw(vertexArray, vertexCount, type);
        }

        static inline void DrawIndexed(
            const Ref<VertexArray>& vertexArray,
            uint32 indexCount,
            PrimitiveType type = PrimitiveType::TRIANGLE
        ) {
            Renderer::GetInstance().DrawIndexed(vertexArray, indexCount, type);
        }

        static inline void DrawInstanced(
            const Ref<VertexArray>& vertexArray,
            uint32 vertexCount,
            uint32 instanceCount,
            PrimitiveType type = PrimitiveType::TRIANGLE
        ) {
            Renderer::GetInstance().DrawInstanced(vertexArray, vertexCount, instanceCount, type);
        }

        static inline void DrawIndexedInstanced(
            const Ref<VertexArray>& vertexArray,
            uint32 indexCount,
            uint32 instanceCount,
            PrimitiveType type = PrimitiveType::TRIANGLE
        ) {
            Renderer::GetInstance().DrawIndexedInstanced(vertexArray, indexCount, instanceCount, type);
        }

    }

}