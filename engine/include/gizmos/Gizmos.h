#pragma once

#include "assets/Mesh.h"
#include "math/Transform.h"
#include "render/Camera.h"
#include "render/objects/Shader.h"

namespace Calyx {

    class Gizmos {
    CX_SINGLETON(Gizmos);

        friend class Application;

    public:
        Gizmos();

    public:
        CX_SINGLETON_EXPOSE_METHOD(
            _DrawWireSphere,
            void DrawWireSphere(const vec3& center, float radius),
            center, radius
        );
        CX_SINGLETON_EXPOSE_METHOD(
            _DrawWireCube,
            void DrawWireCube(const vec3& position, const vec3& size),
            position, size
        );
        CX_SINGLETON_EXPOSE_METHOD(
            _DrawLine,
            void DrawLine(const vec3& start, const vec3& end),
            start, end
        );
        CX_SINGLETON_EXPOSE_METHOD(
            _DrawRay,
            void DrawRay(const vec3& point, const vec3& direction),
            point, direction
        );
        CX_SINGLETON_EXPOSE_METHOD(
            _DrawPoint,
            void DrawPoint(const vec3& point),
            point
        );
        CX_SINGLETON_EXPOSE_METHOD(
            _DrawFrustum,
            void DrawFrustum(const Transform& transform, float aspect, float fov, float nearPlane, float farPlane),
            transform, aspect, fov, nearPlane, farPlane
        );

        CX_SINGLETON_EXPOSE_METHOD(_SetColor, void SetColor(const vec4& color), color);

    public:
        void BeginFrame(const Camera& camera, const Transform& cameraTransform);
        void EndFrame();

    private:
        void _DrawWireSphere(const vec3& center, float radius);
        void _DrawWireCube(const vec3& position, const vec3& size);
        void _DrawLine(const vec3& start, const vec3& end);
        void _DrawRay(const vec3& point, const vec3& direction);
        void _DrawPoint(const vec3& point);
        void _DrawFrustum(const Transform& transform, float aspect, float fov, float nearPlane, float farPlane);

        void _SetColor(const vec4& color) { m_gizmoColor = color; }

        void DrawLines();

    private:
        const Camera* m_camera = nullptr;
        const Transform* m_cameraTransform = nullptr;

        vec4 m_gizmoColor{ 1.0f, 1.0f, 1.0f, 1.0f };
        Ref<Shader> m_gizmoShader;
        Ref<Mesh> m_wireCircleMesh;
        Ref<Mesh> m_wireCubeMesh;
        Scope<Mesh> m_lineMesh;

    };

}