#include "scene/SceneRenderer.h"

namespace Calyx {

    SceneRenderer::SceneRenderer()
        : m_shader(AssetRegistry::LoadAsset<Shader>("assets/shaders/basic.glsl")) {}

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

}