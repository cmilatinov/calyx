#include "render/opengl/GLRenderer.h"

namespace Calyx {

    void GLRenderer::Initialize() {
        // Enable depth test
        glEnable(GL_DEPTH_TEST);

        // Enable back face culling
        glCullFace(GL_BACK);
        glEnable(GL_CULL_FACE);

        // Enable blending
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_BLEND);
    }

    void GLRenderer::SetViewport(uint32 x, uint32 y, uint32 width, uint32 height) {
        glViewport(x, y, width, height);
    }

    void GLRenderer::SetClearColor(const vec4& color) {
        glClearColor(color.r, color.g, color.b, color.a);
    }

    void GLRenderer::Clear() {
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    }

    void GLRenderer::DrawIndexed(const Ref<VertexArray>& vertexArray, uint32 indexCount) {
        vertexArray->Bind();
        glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr);
    }

}