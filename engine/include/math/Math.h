#pragma once

namespace Calyx::Math {

    mat4 ComposeTransform(const vec3& translation, const vec3& rotation, const vec3& scale);

    void DecomposeTransform(const mat4& matrix, vec3& translation, vec3& rotation, vec3& scale);

}