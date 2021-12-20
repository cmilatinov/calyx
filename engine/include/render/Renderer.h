#pragma once

#include "render/objects/VertexArray.h"

namespace Calyx {

    enum class RendererAPI {
        NONE,
        OPENGL,
        DIRECTX,
        VULKAN
    };

    class CALYX_API Renderer {

    public:
        virtual ~Renderer() = default;

        virtual void Initialize() = 0;
        virtual void SetViewport(uint32 x, uint32 y, uint32 width, uint32 height) = 0;
        virtual void SetClearColor(const vec4& color) = 0;
        virtual void Clear() = 0;

        virtual void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32 indexCount) = 0;

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

        static inline void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32 indexCount) {
            Renderer::GetInstance().DrawIndexed(vertexArray, indexCount);
        }

    }

}