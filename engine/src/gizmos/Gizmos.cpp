#include "gizmos/Gizmos.h"
#include "assets/Assets.h"

namespace Calyx {

    CX_SINGLETON_INSTANCE(Gizmos);

    Gizmos::Gizmos()
        : m_wireCircleMesh(Assets::WireCircle()),
          m_wireCubeMesh(Assets::WireCube()),
          m_gizmoShader(AssetRegistry::LoadAsset<Shader>("shaders/gizmos.glsl")),
          m_lineMesh(CreateScope<Mesh>()) {
        m_lineMesh->SetPrimitiveType(PrimitiveType::LINE);
        m_lineMesh->GetInstances().resize(1);
        m_lineMesh->GetInstances()[0] = glm::identity<mat4>();
    }

    void Gizmos::_DrawWireSphere(const vec3& center, float radius) {
        CX_CORE_ASSERT(
            m_camera != nullptr && m_cameraTransform != nullptr,
            "Call to DrawWireSphere() outside of gizmo frame!"
        );
        auto translation = glm::translate(glm::identity<mat4>(), center);
        auto scale = vec3(radius, radius, radius);
        m_gizmoShader->SetFloat4("color", m_gizmoColor);
        m_gizmoShader->SetBool("enableNormals", true);

        auto& instances = m_wireCircleMesh->GetInstances();
        instances.resize(3);
        instances[0] = glm::scale(translation, scale);
        instances[1] = glm::scale(
            glm::rotate(translation, glm::radians(90.0f), vec3(1, 0, 0)),
            scale
        );
        instances[2] = glm::scale(
            glm::rotate(translation, glm::radians(90.0f), vec3(0, 1, 0)),
            scale
        );
        m_wireCircleMesh->RebuildInstances();
        m_wireCircleMesh->DrawInstanced(3);

        auto toCamera = m_cameraTransform->GetPosition() - center;
        auto toCameraNormal = glm::normalize(toCamera);
        auto angles = glm::eulerAngles(
            glm::quatLookAt(
                toCameraNormal,
                vec3(0, 1, 0)
            )
        );
        auto distance = glm::length(toCamera);
        auto alpha = (glm::pi<float>() / 2) - glm::asin(radius / distance);
        auto h = radius * glm::sin(alpha);
        auto l = radius * glm::cos(alpha);
        auto transform = glm::translate(translation, glm::normalize(toCamera) * l);
        transform = glm::rotate(transform, angles.z, vec3(0, 0, 1));
        transform = glm::rotate(transform, angles.y, vec3(0, 1, 0));
        transform = glm::rotate(transform, angles.x, vec3(1, 0, 0));
        transform = glm::scale(transform, vec3(h, h, h));
        m_gizmoShader->SetBool("enableNormals", false);
        instances.resize(1);
        instances[0] = transform;
        m_wireCircleMesh->RebuildInstances();
        m_wireCircleMesh->DrawInstanced(1);
    }

    void Gizmos::_DrawWireCube(const vec3& position, const vec3& size) {
        CX_CORE_ASSERT(
            m_camera != nullptr && m_cameraTransform != nullptr,
            "Call to DrawWireCube() outside of gizmo frame!"
        );
        m_gizmoShader->SetFloat4("color", m_gizmoColor);
        m_gizmoShader->SetBool("ns", false);

        auto& instances = m_wireCubeMesh->GetInstances();
        instances.resize(1);
        instances[0] = glm::translate(glm::identity<mat4>(), position);
        instances[0] = glm::scale(instances[0], size);
        m_wireCubeMesh->RebuildInstances();
        m_wireCubeMesh->DrawIndexedInstanced(1);
    }

    void Gizmos::_DrawLine(const vec3& start, const vec3& end) {
        CX_CORE_ASSERT(
            m_camera != nullptr && m_cameraTransform != nullptr,
            "Call to DrawLine() outside of gizmo frame!"
        );
        m_lineMesh->GetVertices().push_back(start);
        m_lineMesh->GetVertices().push_back(end);
    }

    void Gizmos::_DrawRay(const vec3& point, const vec3& direction) {
        CX_CORE_ASSERT(
            m_camera != nullptr && m_cameraTransform != nullptr,
            "Call to DrawRay() outside of gizmo frame!"
        );
    }

    void Gizmos::_DrawPoint(const vec3& point) {
        CX_CORE_ASSERT(
            m_camera != nullptr && m_cameraTransform != nullptr,
            "Call to DrawPoint() outside of gizmo frame!"
        );
    }

    void Gizmos::_DrawFrustum(
        const Transform& transform,
        float aspect,
        float fov,
        float nearPlane,
        float farPlane
    ) {
        CX_CORE_ASSERT(
            m_camera != nullptr && m_cameraTransform != nullptr,
            "Call to DrawFrustum() outside of gizmo frame!"
        );
        auto camera = Camera(fov, aspect, nearPlane, farPlane);
        mat4 matrix = glm::inverse(camera.GetProjectionMatrix() * transform.GetInverseMatrix());
        vec4 _n1 = matrix * vec4(-1, -1, -1, 1);
        auto n1 = vec3(_n1 / _n1.w);
        vec4 _n2 = matrix * vec4(-1, 1, -1, 1);
        auto n2 = vec3(_n2 / _n2.w);
        vec4 _n3 = matrix * vec4(1, 1, -1, 1);
        auto n3 = vec3(_n3 / _n3.w);
        vec4 _n4 = matrix * vec4(1, -1, -1, 1);
        auto n4 = vec3(_n4 / _n4.w);
        vec4 _f1 = matrix * vec4(-1, -1, 1, 1);
        auto f1 = vec3(_f1 / _f1.w);
        vec4 _f2 = matrix * vec4(-1, 1, 1, 1);
        auto f2 = vec3(_f2 / _f2.w);
        vec4 _f3 = matrix * vec4(1, 1, 1, 1);
        auto f3 = vec3(_f3 / _f3.w);
        vec4 _f4 = matrix * vec4(1, -1, 1, 1);
        auto f4 = vec3(_f4 / _f4.w);
        _DrawLine(n1, n2);
        _DrawLine(n2, n3);
        _DrawLine(n3, n4);
        _DrawLine(n4, n1);
        _DrawLine(f1, f2);
        _DrawLine(f2, f3);
        _DrawLine(f3, f4);
        _DrawLine(f4, f1);
        _DrawLine(f1, n1);
        _DrawLine(f2, n2);
        _DrawLine(f3, n3);
        _DrawLine(f4, n4);
    }

    void Gizmos::BeginFrame(const Camera& camera, const Transform& cameraTransform) {
        m_camera = &camera;
        m_cameraTransform = &cameraTransform;

        auto view = camera.GetProjectionMatrix() * cameraTransform.GetInverseMatrix();
        auto viewPos = cameraTransform.GetPosition();

        m_gizmoShader->Bind();
        m_gizmoShader->SetMat4("view", view);
        m_gizmoShader->SetFloat3("viewPos", viewPos);

        m_lineMesh->GetVertices().clear();
    }

    void Gizmos::DrawLines() {
        m_gizmoShader->SetBool("enableNormals", false);
        const auto nVertices = m_lineMesh->GetVertices().size();
        m_lineMesh->GetNormals().resize(nVertices);
        m_lineMesh->Rebuild();
        m_lineMesh->DrawInstanced(1);
    }

    void Gizmos::EndFrame() {
        DrawLines();

        m_camera = nullptr;
        m_cameraTransform = nullptr;
        m_gizmoShader->Unbind();
    }

}