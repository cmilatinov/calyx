#include "assets/Assets.h"

#include "assets/Mesh.h"
#include "render/objects/Shader.h"
#include "render/objects/Texture2D.h"

namespace Calyx::Assets {

    Ref<Mesh> ScreenSpaceQuad() {
        auto quad = AssetRegistry::LoadAsset<Mesh>(CX_ASSET_SCREEN_SPACE_QUAD);
        if (!quad) {
            quad = AssetRegistry::CreateAsset<Mesh>(CX_ASSET_SCREEN_SPACE_QUAD);
            quad->GetIndices() = {
                0, 1, 2,
                1, 0, 3
            };
            quad->GetVertices() = {
                vec3(-1, -1, 0),
                vec3(1, 1, 0),
                vec3(-1, 1, 0),
                vec3(1, -1, 0)
            };
            quad->GetNormals() = {
                vec3(0, 0, 1),
                vec3(0, 0, 1),
                vec3(0, 0, 1),
                vec3(0, 0, 1)
            };
            quad->GetUV0() = {
                vec2(0, 0),
                vec2(1, 1),
                vec2(0, 1),
                vec2(1, 0)
            };
            quad->Rebuild();
        }
        return quad;
    }

    Ref<Mesh> WireCircle() {
        static constexpr int resolution = 72;
        auto circle = AssetRegistry::LoadAsset<Mesh>(CX_ASSET_WIRE_CIRCLE);
        if (!circle) {
            circle = AssetRegistry::CreateAsset<Mesh>(CX_ASSET_WIRE_CIRCLE);
            auto& vertices = circle->GetVertices();
            auto& normals = circle->GetNormals();
            vertices.reserve(resolution);
            normals.reserve(resolution);
            for (int i = 0; i < resolution; i++) {
                auto angle = glm::radians(i * 360.0f / resolution);
                vertices.emplace_back(glm::cos(angle), glm::sin(angle), 0);
                normals.emplace_back(glm::cos(angle), glm::sin(angle), 0);
            }
            circle->SetPrimitiveType(PrimitiveType::LINE_LOOP);
            circle->Rebuild();
        }
        return circle;
    }

    Ref<Mesh> WireCube() {
        auto square = AssetRegistry::LoadAsset<Mesh>(CX_ASSET_WIRE_CUBE);
        if (!square) {
            square = AssetRegistry::CreateAsset<Mesh>(CX_ASSET_WIRE_CUBE);
            square->GetIndices() = {
                0, 1, 1, 2, 2, 3, 3, 0,
                4, 5, 5, 6, 6, 7, 7, 4,
                0, 4, 1, 5, 2, 6, 3, 7
            };
            square->GetVertices() = {
                vec3(-0.5f, -0.5f, -0.5f),
                vec3(-0.5f, 0.5f, -0.5f),
                vec3(0.5f, 0.5f, -0.5f),
                vec3(0.5f, -0.5f, -0.5f),
                vec3(-0.5f, -0.5f, 0.5f),
                vec3(-0.5f, 0.5f, 0.5f),
                vec3(0.5f, 0.5f, 0.5f),
                vec3(0.5f, -0.5f, 0.5f)
            };
            square->GetNormals() = {
                vec3(0, 0, 0),
                vec3(0, 0, 0),
                vec3(0, 0, 0),
                vec3(0, 0, 0),
                vec3(0, 0, 0),
                vec3(0, 0, 0),
                vec3(0, 0, 0),
                vec3(0, 0, 0)
            };
            square->SetPrimitiveType(PrimitiveType::LINE);
            square->Rebuild();
        }
        return square;
    }

}