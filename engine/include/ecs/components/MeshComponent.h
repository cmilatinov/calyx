#pragma once

#include "ecs/Component.h"
#include "assets/Mesh.h"

namespace Calyx {

    class MeshComponent : public Component<MeshComponent> {
        CX_BEGIN_REFLECT();

    public:
        explicit MeshComponent(Mesh* mesh)
            : m_mesh(mesh) {}

        String GetName() const override { return "Mesh Component"; }

        void SetMesh(Mesh* mesh) { m_mesh = mesh; }

        void DrawMesh() const {
            if (m_mesh == nullptr) {
                CX_CORE_WARN("Cannot draw null mesh!");
                return;
            }
            m_mesh->Draw();
        }

    private:
        Mesh* m_mesh;

    };

}