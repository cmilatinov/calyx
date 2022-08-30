#include "math/ScreenSpaceUtils.h"

namespace Calyx::ScreenSpaceUtils {

    bool IsInBounds(const vec2& point, const vec4& bounds) {
        return point.x >= bounds.x &&
               point.x <= bounds.z &&
               point.y >= bounds.y &&
               point.y <= bounds.w;
    }

}