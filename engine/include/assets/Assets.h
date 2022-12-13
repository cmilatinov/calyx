#pragma once

#include "assets/AssetRegistry.h"

#define CX_ASSET_SCREEN_SPACE_QUAD "screen_space_quad"

namespace Calyx {
    class Mesh;
}

namespace Calyx::Assets {

    Ref<Mesh> ScreenSpaceQuad();

}