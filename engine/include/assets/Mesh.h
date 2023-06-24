#pragma once

#include "render/Renderer.h"
#include "render/objects/VertexArray.h"
#include "assets/AssetRegistry.h"

#define CX_MESH_VERTICES    0
#define CX_MESH_NORMALS     1
#define CX_MESH_UV0         2
#define CX_MESH_UV1         3
#define CX_MESH_UV2         4
#define CX_MESH_UV3         5
#define CX_MESH_INSTANCES   6

namespace Calyx {

    class CALYX_API Mesh : public Asset<Mesh> {
        CX_ASSET_REGISTRY_FRIEND();
        CX_BEGIN_REFLECT();
        CX_REFLECT_CONVERTIBLE_REF(IAsset);

    public:
        void Clear();
        bool Load(const String& file);

        void Draw() const;
        void DrawIndexed() const;
        void DrawInstanced(uint32 instanceCount) const;
        void DrawIndexedInstanced(uint32 instanceCount) const;

        void Rebuild();
        void RebuildInstances();

        List<uint32>& GetIndices() { return m_indices; }
        List<vec3>& GetVertices() { return m_vertices; }
        List<vec3>& GetNormals() { return m_normals; }
        List<vec2>& GetUV0() { return m_uvs0; }
        List<vec2>& GetUV1() { return m_uvs1; }
        List<vec2>& GetUV2() { return m_uvs2; }
        List<vec2>& GetUV3() { return m_uvs3; }
        List<mat4>& GetInstances() { return m_instances; }

        void SetPrimitiveType(PrimitiveType type) { m_primitiveType = type; }

    private:
        PrimitiveType m_primitiveType = PrimitiveType::TRIANGLE;
        List<uint32> m_indices;
        List<vec3> m_vertices;
        List<vec3> m_normals;
        List<vec2> m_uvs0;
        List<vec2> m_uvs1;
        List<vec2> m_uvs2;
        List<vec2> m_uvs3;
        List<mat4> m_instances;
        Ref<VertexArray> m_vertexArray;

        void CreateVAO();

        static Mesh* Create(const String& file);

    };

}