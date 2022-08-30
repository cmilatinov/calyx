#include "math/Math.h"

namespace Calyx::Math {

    mat4 ComposeTransform(const vec3& translation, const vec3& rotation, const vec3& scale) {
        mat4 matrix = glm::identity<mat4>();
        matrix = glm::translate(matrix, translation);
        matrix = glm::rotate(matrix, glm::radians(rotation.z), vec3(0, 0, 1));
        matrix = glm::rotate(matrix, glm::radians(rotation.y), vec3(0, 1, 0));
        matrix = glm::rotate(matrix, glm::radians(rotation.x), vec3(1, 0, 0));
        matrix = glm::scale(matrix, scale);
        return matrix;
    }

    void DecomposeTransform(const mat4& matrix, vec3& translation, vec3& rotation, vec3& scale) {
        mat3 rotationMatrix(matrix);

        // Position
        translation = vec3(matrix[3]);

        // Scale
        vec3 column0 = glm::column(rotationMatrix, 0);
        vec3 column1 = glm::column(rotationMatrix, 1);
        vec3 column2 = glm::column(rotationMatrix, 2);
        float sx = glm::length(column0);
        float sy = glm::length(column1);
        float sz = glm::length(column2);
        scale = vec3(sx, sy, sz);

        // Rotation
        rotationMatrix = mat3(column0 / sx, column1 / sy, column2 / sz);
        rotation = glm::degrees(glm::eulerAngles(glm::quat_cast(rotationMatrix)));
    }

}