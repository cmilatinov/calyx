#pragma once

#include <imgui.h>
#include <Calyx.h>

#include "input/Input.h"
#include "render/CameraEditor.h"

namespace Calyx {

    class EditorLayer : public ILayer {

    public:
        EditorLayer();

        void OnAttach() override;
        void OnUpdate() override;
        void OnGUI() override;
        void OnEvent(Event& event) override;

        virtual String GetName() const override { return "Test Layer"; };

    private:
        Ref<Shader> shader;
        Ref<VertexArray> vertexArray;
        Ref<IndexBuffer> indexBuffer;
        Ref<VertexBuffer> vertexBuffer;
        Ref<Framebuffer> framebuffer;
        Ref<Framebuffer> msaaFramebuffer;
        Scope<CameraEditor> m_editorCamera;

    };

}