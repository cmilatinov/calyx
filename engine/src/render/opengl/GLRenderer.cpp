#include "render/opengl/GLRenderer.h"
#include "render/opengl/GLUtils.h"

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

    void GLRenderer::Draw(
        const Ref<VertexArray>& vertexArray,
        uint32 vertexCount,
        PrimitiveType type
    ) {
        vertexArray->Bind();
        glDrawArrays(GLPrimitiveType(type), 0, vertexCount);
    }

    void GLRenderer::DrawIndexed(
        const Ref<VertexArray>& vertexArray,
        uint32 indexCount,
        PrimitiveType type
    ) {
        vertexArray->Bind();
        glDrawElements(GLPrimitiveType(type), indexCount, GL_UNSIGNED_INT, nullptr);
    }

    void GLRenderer::DrawInstanced(
        const Ref<VertexArray>& vertexArray,
        uint32 vertexCount,
        uint32 instanceCount,
        PrimitiveType type
    ) {
        vertexArray->Bind();
        glDrawArraysInstanced(GLPrimitiveType(type), 0, vertexCount, instanceCount);
    }

    void GLRenderer::DrawIndexedInstanced(
        const Ref<VertexArray>& vertexArray,
        uint32 indexCount,
        uint32 instanceCount,
        PrimitiveType type
    ) {
        vertexArray->Bind();
        glDrawElementsInstanced(GLPrimitiveType(type), indexCount, GL_UNSIGNED_INT, nullptr, instanceCount);
    }

}