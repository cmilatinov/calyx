#pragma once

#include "ecs/Component.h"
#include "assets/Mesh.h"

namespace Calyx {

    class CALYX_API MeshComponent final : public Component<MeshComponent> {
        CX_BEGIN_REFLECT();

    public:
        MeshComponent() = default;

        String GetDisplayName() const override { return "Mesh Renderer"; }

        Ref<Mesh> GetMesh() const { return m_mesh; }

        void SetMesh(const Ref<Mesh>& mesh) { m_mesh = mesh; }

    private:
        CX_PROPERTY(CX_SERIALIZE, CX_NAME(Mesh))
        Ref<Mesh> m_mesh;

    };

}