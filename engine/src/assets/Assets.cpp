#include "assets/Assets.h"

#include "assets/Mesh.h"
#include "render/objects/Shader.h"
#include "render/objects/Texture2D.h"

namespace Calyx::Assets {

    Ref<Mesh> ScreenSpaceQuad() {
        auto quad = AssetRegistry::LoadAsset<Mesh>(CX_ASSET_SCREEN_SPACE_QUAD);
        if (!quad) {
            quad = AssetRegistry::CreateAsset<Mesh>(CX_ASSET_SCREEN_SPACE_QUAD);
            quad->SetIndices(
                {
                    0, 1, 2,
                    1, 0, 3
                }
            );
            quad->SetVertices(
                {
                    vec3(-1, -1, 0),
                    vec3(1, 1, 0),
                    vec3(-1, 1, 0),
                    vec3(1, -1, 0)
                }
            );
            quad->SetNormals(
                {
                    vec3(0, 0, 1),
                    vec3(0, 0, 1),
                    vec3(0, 0, 1),
                    vec3(0, 0, 1)
                }
            );
            quad->SetUV0(
                {
                    vec2(0, 0),
                    vec2(1, 1),
                    vec2(0, 1),
                    vec2(1, 0)
                }
            );
        }
        return quad;
    }

}