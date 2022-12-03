#include "assets/Assets.h"

#include "assets/Mesh.h"
#include "render/objects/Shader.h"
#include "render/objects/Texture2D.h"

namespace Calyx::Assets {

    void InitAssetTypes() {
        AssetRegistry::RegisterAssetType<Mesh>(".obj", ".fbx", ".3ds", ".blend", ".ply");
        AssetRegistry::RegisterAssetType<Shader>(".glsl");
        AssetRegistry::RegisterAssetType<Texture2D>(".png", ".jpg", ".bmp");
    }

    WeakRef<Mesh> ScreenSpaceQuad() {
        auto quad = AssetRegistry::LoadAsset<Mesh>(CX_ASSET_SCREEN_SPACE_QUAD);
        if (quad.expired()) {
            quad = AssetRegistry::CreateAsset<Mesh>(CX_ASSET_SCREEN_SPACE_QUAD);
            if (CX_LOCK_PTR(quad, quadPtr)) {
                quadPtr->SetIndices(
                    {
                        0, 1, 2,
                        1, 0, 3
                    }
                );
                quadPtr->SetVertices(
                    {
                        vec3(-1, -1, 0),
                        vec3(1, 1, 0),
                        vec3(-1, 1, 0),
                        vec3(1, -1, 0)
                    }
                );
            }
        }
        return quad;
    }

}