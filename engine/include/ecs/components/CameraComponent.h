#pragma once

#include "ecs/Component.h"
#include "render/Camera.h"
#include "render/Color.h"
#include "gizmos/Gizmos.h"

namespace Calyx {

    class CALYX_API CameraComponent final : public Component<CameraComponent> {
        CX_BEGIN_REFLECT();

    public:
        Camera& GetCamera() { return m_camera; }

        String GetDisplayName() const override { return "Camera Component"; }

    private:
        void CX_DRAW_GIZMOS_SELECTED() {
            Gizmos::SetColor(Color::WHITE);
            Gizmos::DrawFrustum(
                m_gameObject->GetTransform(),
                m_camera.GetAspect(),
                m_camera.GetFOV(),
                m_camera.GetNearPlane(),
                m_camera.GetFarPlane()
            );
        }

        void CX_DRAW_GIZMOS() {
            Gizmos::SetColor(vec4(1, 1, 1, 0.5));
            Gizmos::DrawFrustum(
                m_gameObject->GetTransform(),
                m_camera.GetAspect(),
                m_camera.GetFOV(),
                m_camera.GetNearPlane(),
                m_camera.GetFarPlane()
            );
        }

    private:
        CX_PROPERTY(CX_SERIALIZE)
        Camera m_camera;

    };

}