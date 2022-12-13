#pragma once

#include "render/objects/VertexArray.h"
#include "assets/AssetRegistry.h"

namespace Calyx {

    class CALYX_API Mesh : public Asset<Mesh> {
        CX_ASSET_REGISTRY_FRIEND();
        CX_BEGIN_REFLECT();
        CX_REFLECT_CONVERTIBLE_REF(IAsset);

    public:
        void Clear();
        bool Load(const String& file);
        bool Load(const List<uint32>& indices, const List<vec3>& vertices);

        void Draw() const;

        void SetIndices(const List<uint32>& indices);
        void SetVertices(const List<vec3>& vertices);
        void SetNormals(const List<vec3>& normals);
        void SetUV0(const List<vec2>& uvs0);
        void SetUV1(const List<vec2>& uvs1);
        void SetUV2(const List<vec2>& uvs2);
        void SetUV3(const List<vec2>& uvs3);

    private:
        List<uint32> m_indices;
        List<vec3> m_vertices;
        List<vec3> m_normals;
        List<vec2> m_uvs0;
        List<vec2> m_uvs1;
        List<vec2> m_uvs2;
        List<vec2> m_uvs3;
        Ref<VertexArray> m_vertexArray;

        void CreateVAO();
        void UpdateVAO();

        static Mesh* Create(const String& file);

    };

}