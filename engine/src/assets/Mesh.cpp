#include "assets/Mesh.h"
#include "render/Renderer.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace Calyx {

    void Mesh::Clear() {
        m_indices.clear();
        m_indices.shrink_to_fit();

        m_vertices.clear();
        m_vertices.shrink_to_fit();

        m_normals.clear();
        m_normals.shrink_to_fit();

        m_uvs0.clear();
        m_uvs0.shrink_to_fit();

        m_vertexArray.reset();
    }

    bool Mesh::Load(const String& path) {
        Assimp::Importer importer;

        const auto* scene = importer.ReadFile(
            path,
            aiProcess_Triangulate |
            aiProcess_GenSmoothNormals |
            aiProcess_FlipUVs |
            aiProcess_JoinIdenticalVertices
        );
        if (scene == nullptr || scene->mNumMeshes <= 0)
            return false;

        aiMesh* mesh = scene->mMeshes[0];

        // Copy indices
        m_indices.resize(mesh->mNumFaces * 3);
        for (int i = 0; i < mesh->mNumFaces; i++) {
            for (int j = 0; j < mesh->mFaces[i].mNumIndices; j++) {
                m_indices[(i * 3) + j] = mesh->mFaces[i].mIndices[j];
            }
        }

        // Resize vertex, normal and UV vectors
        m_vertices.resize(mesh->mNumVertices);
        m_normals.resize(mesh->mNumVertices);
        int numUVs = std::min((int) mesh->GetNumUVChannels(), 4);
        List<vec2>* uvChannels[4] = { &m_uvs0, &m_uvs1, &m_uvs2, &m_uvs3 };
        for (int j = 0; j < numUVs; j++) {
            uvChannels[j]->resize(mesh->mNumVertices);
        }

        // Copy vertices, normals, and UVs
        for (int i = 0; i < mesh->mNumVertices; i++) {
            auto& vertex = mesh->mVertices[i];
            m_vertices[i] = vec3(vertex.x, vertex.y, vertex.z);
            auto& normal = mesh->mNormals[i];
            m_normals[i] = vec3(normal.x, normal.y, normal.z);

            for (int j = 0; j < numUVs; j++) {
                auto& uv = mesh->mTextureCoords[j][i];
                (*uvChannels[j])[i] = vec2(uv.x, uv.y);
            }
        }

        UpdateVAO();

        return true;
    }

    bool Mesh::Load(const List<uint32>& indices, const List<vec3>& vertices) {
        m_indices = indices;
        m_vertices = vertices;

        UpdateVAO();

        return true;
    }

    void Mesh::Draw() const {
        RenderCommand::DrawIndexed(m_vertexArray, m_indices.size());
    }

    void Mesh::CreateVAO() {
        m_vertexArray = VertexArray::Create();
        m_vertexArray->Bind();

        auto indexBuffer = IndexBuffer::Create(m_indices.size(), m_indices.data());
        m_vertexArray->SetIndexBuffer(indexBuffer);

        auto vertexBuffer = VertexBuffer::Create(
            m_vertices.size() * sizeof(vec3),
            reinterpret_cast<float*>(m_vertices.data())
        );
        vertexBuffer->SetLayout(
            {
                { ShaderDataType::Float3, "vertex" }
            }
        );
        m_vertexArray->AddVertexBuffer(vertexBuffer);

        auto normalBuffer = VertexBuffer::Create(
            m_normals.size() * sizeof(vec3),
            reinterpret_cast<float*>(m_normals.data())
        );
        normalBuffer->SetLayout(
            {
                { ShaderDataType::Float3, "normal" }
            }
        );
        m_vertexArray->AddVertexBuffer(normalBuffer);

        List<vec2>* uvs[4] = { &m_uvs0, &m_uvs1, &m_uvs2, &m_uvs3 };
        for (int i = 0; i < 4; i++) {
            auto uvBuffer = VertexBuffer::Create(
                uvs[i]->size() * sizeof(vec2),
                reinterpret_cast<float*>(uvs[i]->data())
            );
            uvBuffer->SetLayout(
                {
                    { ShaderDataType::Float2, String("uv") + std::to_string(i) },
                }
            );
            m_vertexArray->AddVertexBuffer(uvBuffer);
        }

        m_vertexArray->Unbind();
    }

    void Mesh::UpdateVAO() {
        if (m_vertexArray == nullptr) {
            return CreateVAO();
        }

        m_vertexArray->Bind();
        m_vertexArray->GetIndexBuffer()->SetData(m_indices.size(), m_indices.data());
        const auto& buffers = m_vertexArray->GetVertexBuffers();
        buffers[0]->SetData(m_vertices.size() * sizeof(vec3), m_vertices.data());
        buffers[1]->SetData(m_normals.size() * sizeof(vec3), m_normals.data());
        buffers[2]->SetData(m_uvs0.size() * sizeof(vec2), m_uvs0.data());
        buffers[3]->SetData(m_uvs1.size() * sizeof(vec2), m_uvs1.data());
        buffers[4]->SetData(m_uvs2.size() * sizeof(vec2), m_uvs2.data());
        buffers[5]->SetData(m_uvs3.size() * sizeof(vec2), m_uvs3.data());
        m_vertexArray->Unbind();
    }

    void Mesh::SetIndices(const List<uint32>& indices) {
        m_indices = indices;
        UpdateVAO();
    }

    void Mesh::SetVertices(const List<vec3>& vertices) {
        m_vertices = vertices;
        UpdateVAO();
    }

    void Mesh::SetNormals(const List<vec3>& normals) {
        m_normals = normals;
        UpdateVAO();
    }

    void Mesh::SetUV0(const List<vec2>& uvs0) {
        m_uvs0 = uvs0;
        UpdateVAO();
    }

    void Mesh::SetUV1(const List<vec2>& uvs1) {
        m_uvs1 = uvs1;
        UpdateVAO();
    }

    void Mesh::SetUV2(const List<vec2>& uvs2) {
        m_uvs2 = uvs2;
        UpdateVAO();
    }

    void Mesh::SetUV3(const List<vec2>& uvs3) {
        m_uvs3 = uvs3;
        UpdateVAO();
    }

    Mesh* Mesh::Create(const String& file) {
        auto* mesh = new Mesh();
        if (!mesh->Load(file))
            return nullptr;
        return mesh;
    }

}