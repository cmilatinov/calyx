#include "scene/SceneRenderer.h"
#include "assets/Assets.h"

#include <glad/glad.h>

namespace Calyx {

    SceneRenderer::SceneRenderer()
        : m_shader(AssetRegistry::LoadAsset<Shader>("shaders/pbr.glsl")),
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
            auto mesh = meshComponent.GetMesh();
            if (!mesh) continue;

            GameObject* gameObject = scene.m_gameObjects.find(entity)->second.get();
            auto& instances = mesh->GetInstances();
            instances.resize(1);
            instances[0] = gameObject->GetTransform().GetMatrix(m_transformCache);
            mesh->RebuildInstances();
            mesh->DrawIndexedInstanced(1);
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
        m_screenSpaceQuad->DrawIndexed();
        m_gridShader->Unbind();
    }

}