#pragma once

#include "ecs/Component.h"
#include "assets/Mesh.h"

namespace Calyx {

    class MeshComponent : public Component<MeshComponent> {
        CX_BEGIN_REFLECT();

    public:
        MeshComponent()
            : m_mesh() {}

        explicit MeshComponent(const Ref<Mesh>& mesh)
            : m_mesh(mesh) {}

        String GetName() const override { return "Mesh Component"; }

        void SetMesh(const Ref<Mesh>& mesh) { m_mesh = mesh; }

        void DrawMesh() const {
            if (!m_mesh) return;
            m_mesh->Draw();
        }

    private:
        CX_REFLECT_SERIALIZE
        CX_REFLECT_NAME("Mesh")
        Ref<Mesh> m_mesh;

    };

}