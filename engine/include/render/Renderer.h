#pragma once

namespace Calyx {

    class CALYX_API Renderer {

    public:
        enum API {
            NONE,
            OPENGL,
            DIRECTX,
            VULKAN
        };

    public:
        virtual ~Renderer() = default;

        virtual void Initialize() = 0;
        virtual void SetViewport(uint32 x, uint32 y, uint32 width, uint32 height) = 0;
        virtual void SetClearColor(const vec4& color) = 0;
        virtual void Clear() = 0;

//        virtual void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32 indexCount = 0) override;

        static API GetAPI() {
            return s_api;
        }

        static void Init();

        static Scope<Renderer> s_renderer;

    private:
        static API s_api;

        static Scope<Renderer> Create();

    };

    Scope<Renderer>

}