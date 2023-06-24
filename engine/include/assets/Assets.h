#pragma once

#include "assets/AssetRegistry.h"

#define CX_ASSET_SCREEN_SPACE_QUAD  "screen_space_quad"
#define CX_ASSET_WIRE_CIRCLE        "wire_circle"
#define CX_ASSET_WIRE_CUBE          "wire_cube"

namespace Calyx {
    class Mesh;
}

namespace Calyx::Assets {

    Ref<Mesh> ScreenSpaceQuad();

    Ref<Mesh> WireCircle();
    Ref<Mesh> WireCube();

}