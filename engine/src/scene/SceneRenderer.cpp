#include "scene/SceneRenderer.h"
#include "assets/Assets.h"

namespace Calyx {

    SceneRenderer::SceneRenderer()
        : m_shader(AssetRegistry::LoadAsset<Shader>("shaders/basic.glsl")),
          m_gridShader(AssetRegistry::LoadAsset<Shader>("shaders/grid.glsl")),
          m_screenSpaceQuad(Assets::ScreenSpaceQuad()) {}

    void SceneRenderer::RenderScene(const Camera& camera, const Transform& cameraTransform, const Scene& scene) {
        m_transformCache.clear();

        // Compute camera view matrix
        mat4 view = camera.GetProjectionMatrix() * cameraTransform.GetInverseMatrix();
        m_shader->Bind();
        m_shader->SetMat4("view", view);

        // Draw scene meshes
        for (auto [entity, meshComponent]: scene.m_entityRegistry.view<MeshComponent>().each()) {
            GameObject* gameObject = scene.m_gameObjects.find(entity)->second.get();
            mat4 model = gameObject->GetTransform().GetMatrix(m_transformCache);
            m_shader->SetMat4("model", model);
            meshComponent.DrawMesh();
        }

        m_shader->Unbind();
    }

    void SceneRenderer::RenderGrid(const Camera& camera, const Transform& cameraTransform) {
        mat4 view = camera.GetProjectionMatrix() * cameraTransform.GetInverseMatrix();
        mat4 invView = glm::inverse(view);
        m_gridShader->Bind();
        m_gridShader->SetMat4("view", view);
        m_gridShader->SetMat4("invView", invView);
        m_gridShader->SetFloat("nearPlane", camera.GetNearPlane());
        m_gridShader->SetFloat("farPlane", camera.GetFarPlane());
        m_screenSpaceQuad->Draw();
        m_gridShader->Unbind();
    }

}