#include "assets/Mesh.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace Calyx {

    void Mesh::Clear() {
        m_indices.clear();
        m_vertices.clear();
        m_normals.clear();
        m_uvs0.clear();
        m_uvs1.clear();
        m_uvs2.clear();
        m_uvs3.clear();
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

        Clear();
        aiMesh* mesh = scene->mMeshes[0];

        // Copy indices
        m_indices.resize(mesh->mNumFaces * 3);
        for (int i = 0; i < mesh->mNumFaces; i++) {
            for (int j = 0; j < mesh->mFaces[i].mNumIndices; j++) {
                m_indices[(i * 3) + j] = mesh->mFaces[i].mIndices[j];
            }
        }

        // Resize vertex, normal, colors and UV vectors
        m_vertices.resize(mesh->mNumVertices);
        m_normals.resize(mesh->mNumVertices);
        int numUVs = std::min((int)mesh->GetNumUVChannels(), CX_MESH_UV_CHANNELS);
        List<vec2>* uvChannels[CX_MESH_UV_CHANNELS] = { &m_uvs0, &m_uvs1, &m_uvs2, &m_uvs3 };
        for (int i = 0; i < numUVs; i++) {
            uvChannels[i]->resize(mesh->mNumVertices);
        }
        for (int i = numUVs; i < CX_MESH_UV_CHANNELS; i++) {
            SetAttribEnabled(CX_MESH_UV0 + i, false);
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

        Rebuild();

        return true;
    }

    void Mesh::Draw() const {
        RenderCommand::Draw(m_vertexArray, m_vertices.size(), m_primitiveType);
    }

    void Mesh::DrawIndexed() const {
        RenderCommand::DrawIndexed(m_vertexArray, m_indices.size(), m_primitiveType);
    }

    void Mesh::DrawInstanced(uint32 instanceCount) const {
        RenderCommand::DrawInstanced(m_vertexArray, m_vertices.size(), instanceCount, m_primitiveType);
    }

    void Mesh::DrawIndexedInstanced(uint32 instanceCount) const {
        RenderCommand::DrawIndexedInstanced(m_vertexArray, m_indices.size(), instanceCount, m_primitiveType);
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

        List<vec2>* uvs[CX_MESH_UV_CHANNELS] = { &m_uvs0, &m_uvs1, &m_uvs2, &m_uvs3 };
        for (int i = 0; i < CX_MESH_UV_CHANNELS; i++) {
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

        auto instanceBuffer = VertexBuffer::Create(
            m_instances.size() * sizeof(mat4),
            glm::value_ptr(*m_instances.data())
        );
        instanceBuffer->SetLayout(
            {
                { ShaderDataType::Mat4, "transform" }
            }
        );
        m_vertexArray->AddVertexBuffer(instanceBuffer);

        for (uint32 i = 0; i < CX_MESH_NUM_ATTRIBS; i++) {
            m_vertexArray->SetVertexBufferEnabled(i, m_enabledAttribs.test(i));
        }

        m_vertexArray->Unbind();
    }

    void Mesh::Rebuild() {
        if (!m_vertexArray) {
            return CreateVAO();
        }

        m_vertexArray->Bind();
        m_vertexArray->GetIndexBuffer()->SetData(m_indices.size(), m_indices.data());
        const auto& buffers = m_vertexArray->GetVertexBuffers();
        buffers[CX_MESH_VERTICES]->SetData(m_vertices.size() * sizeof(vec3), m_vertices.data());
        buffers[CX_MESH_NORMALS]->SetData(m_normals.size() * sizeof(vec3), m_normals.data());
        buffers[CX_MESH_UV0]->SetData(m_uvs0.size() * sizeof(vec2), m_uvs0.data());
        buffers[CX_MESH_UV1]->SetData(m_uvs1.size() * sizeof(vec2), m_uvs1.data());
        buffers[CX_MESH_UV2]->SetData(m_uvs2.size() * sizeof(vec2), m_uvs2.data());
        buffers[CX_MESH_UV3]->SetData(m_uvs3.size() * sizeof(vec2), m_uvs3.data());
        buffers[CX_MESH_INSTANCES]->SetData(m_instances.size() * sizeof(mat4), m_instances.data());
        for (uint32 i = 0; i < CX_MESH_NUM_ATTRIBS; i++) {
            m_vertexArray->SetVertexBufferEnabled(i, m_enabledAttribs.test(i));
        }
        m_vertexArray->Unbind();
    }

    void Mesh::RebuildInstances() {
        if (!m_vertexArray) {
            return CreateVAO();
        }

        m_vertexArray->Bind();
        const auto& buffers = m_vertexArray->GetVertexBuffers();
        buffers[CX_MESH_INSTANCES]->SetData(m_instances.size() * sizeof(mat4), m_instances.data());
        m_vertexArray->Unbind();
    }

    void Mesh::SetAttribEnabled(uint32 index, bool enabled) {
        m_enabledAttribs.set(index, enabled);
    }

    Mesh* Mesh::Create(const String& file) {
        auto* mesh = new Mesh();
        if (!mesh->Load(file)) {
            delete mesh;
            return nullptr;
        }
        return mesh;
    }

}