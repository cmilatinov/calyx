#pragma once

#include "ecs/Component.h"
#include "assets/Mesh.h"

namespace Calyx {

    class MeshComponent : public Component<MeshComponent> {

    public:
        explicit MeshComponent(Mesh* mesh)
            : m_mesh(mesh) {}

        void SetMesh(Mesh* mesh) { m_mesh = mesh; }

        void DrawMesh() const {
            CX_CORE_ASSERT(m_mesh != nullptr, "Cannot draw null mesh!");
            m_mesh->Draw();
        }

    private:
        Mesh* m_mesh;

    };

}