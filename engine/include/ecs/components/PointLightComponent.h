#pragma once

#include "ecs/ComponentDefinitions.h"
#include "ecs/Component.h"
#include "render/Color.h"
#include "gizmos/Gizmos.h"

namespace Calyx {

    class PointLightComponent : public Component<PointLightComponent> {
        CX_BEGIN_REFLECT();

    public:
        PointLightComponent() = default;

        String GetDisplayName() const override {
            return "Point Light";
        }

    private:
        void CX_DRAW_GIZMOS_SELECTED() {
            auto& transform = m_gameObject->GetTransform();
            Gizmos::SetColor(m_color);
            Gizmos::DrawWireSphere(transform.GetPosition(), m_radius);
//            Gizmos::DrawLine(transform.GetPosition(), transform.GetPosition() + m_radius * transform.Forward());
        }

        void CX_DRAW_GIZMOS() {
            Gizmos::SetColor(vec4(m_color.r, m_color.g, m_color.b, m_color.a * 0.5f));
            Gizmos::DrawWireSphere(m_gameObject->GetTransform().GetPosition(), m_radius);
        }

    private:
        CX_PROPERTY(CX_SERIALIZE, CX_NAME(Color))
        vec4 m_color = Color::WHITE;

        CX_PROPERTY(CX_SERIALIZE, CX_NAME(Radius))
        float m_radius = 10.0f;

    };

}