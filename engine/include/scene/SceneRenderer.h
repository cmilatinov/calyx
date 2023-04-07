#pragma once

#include "render/objects/Shader.h"
#include "render/objects/Framebuffer.h"
#include "render/Camera.h"

#include "ecs/GameObject.h"
#include "ecs/components/MeshComponent.h"

#include "scene/Scene.h"

namespace Calyx {

    class CALYX_API SceneRenderer {

    public:
        SceneRenderer();

        void RenderScene(const Camera& camera, const Transform& cameraTransform, const Scene& scene);
        void RenderGrid(const Camera& camera, const Transform& cameraTransform);

    private:
        Ref<Shader> m_shader;
        Ref<Shader> m_gridShader;

        Ref<Mesh> m_screenSpaceQuad;

        Map<const Transform*, mat4> m_transformCache;
        Map<const Transform*, mat4> m_inverseTransformCache;
    };

}