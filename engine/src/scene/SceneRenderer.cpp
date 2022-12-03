#include "scene/SceneRenderer.h"

namespace Calyx {

    SceneRenderer::SceneRenderer()
        : m_shader(AssetRegistry::LoadAsset<Shader>("shaders/basic.glsl")) {}

    void SceneRenderer::RenderScene(const Camera& camera, const Transform& cameraTransform, const Scene& scene) {
        m_transformCache.clear();

        // Compute camera view matrix
        mat4 view = camera.GetProjectionMatrix() * cameraTransform.GetInverseMatrix();
        if (CX_LOCK_PTR(m_shader, shader)) {
            shader->Bind();
            shader->SetMat4("view", view);

            // Draw scene meshes
            for (auto [entity, meshComponent]: scene.m_entityRegistry.view<MeshComponent>().each()) {
                GameObject* gameObject = scene.m_gameObjects.find(entity)->second.get();
                mat4 model = gameObject->GetTransform().GetMatrix(m_transformCache);
                shader->SetMat4("model", model);
                meshComponent.DrawMesh();
            }

            shader->Unbind();
        }
    }

}