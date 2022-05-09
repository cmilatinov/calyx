#include "math/Transform.h"

namespace Calyx {

    Transform::Transform()
        : m_position(0, 0, 0),
        m_rotation(0, 0, 0),
        m_scale(1, 1, 1) {
        UpdateMatrix();
    }

    Transform::Transform(const vec3& position, const vec3& rotation, const vec3& scale)
        : m_position(position),
        m_rotation(rotation),
        m_scale(scale) {
        UpdateMatrix();
    }

    Transform::Transform(const mat4& matrix)
        : m_matrix(matrix) {
        UpdateComponents();
    }

    vec3 Transform::TransformPosition(const vec3& position) const {
        return vec3(GetMatrix() * vec4(position, 1));
    }

    vec3 Transform::TransformDirection(const vec3& direction) const {
        return mat3(GetMatrix()) * direction;
    }

    vec3 Transform::InverseTransformPosition(const vec3& position) const {
        mat4 matrix = GetInverseMatrix();
        return vec3(matrix * vec4(position, 1));
    }

    vec3 Transform::InverseTransformDirection(const vec3& direction) const {
        mat4 matrix = GetInverseMatrix();
        return mat3(matrix) * direction;
    }

    void Transform::SetPosition(const vec3& position) {
        m_position = position;
        UpdateMatrix();
    }

    void Transform::SetRotation(const vec3& rotation) {
        m_rotation = rotation;
        UpdateMatrix();
    }

    void Transform::SetScale(const vec3& scale) {
        m_scale = scale;
        UpdateMatrix();
    }

    void Transform::SetMatrix(const mat4& matrix) {
        m_matrix = matrix;
        UpdateComponents();
    }

    mat4 Transform::GetMatrix() const {
        if (m_parent != nullptr)
            return m_parent->GetMatrix() * m_matrix;
        return m_matrix;
    }

    mat4 Transform::GetInverseMatrix() const {
        if (m_parent != nullptr)
            return glm::inverse(m_matrix) * m_parent->GetInverseMatrix();
        return glm::inverse(m_matrix);
    }

    void Transform::UpdateMatrix() {
        m_matrix = mat4();
        glm::translate(m_matrix, m_position);
        glm::rotate(m_matrix, m_rotation.x, vec3(1, 0, 0));
        glm::rotate(m_matrix, m_rotation.y, vec3(0, 1, 0));
        glm::rotate(m_matrix, m_rotation.z, vec3(0, 0, 1));
        glm::scale(m_matrix, m_scale);
    }

    void Transform::UpdateComponents() {
        mat3 rotationMatrix(m_matrix);

        // Position
        m_position = vec3(m_matrix[3]);

        // Scale
        vec3 column0 = glm::column(rotationMatrix, 0);
        vec3 column1 = glm::column(rotationMatrix, 1);
        vec3 column2 = glm::column(rotationMatrix, 2);
        float sx = column0.length();
        float sy = column1.length();
        float sz = column2.length();
        m_scale = vec3(sx, sy, sz);

        // Rotation
        rotationMatrix = mat3(column0 / sx, column1 / sy, column2 / sz);
        quat rotation = glm::quat_cast(rotationMatrix);
        m_rotation = glm::eulerAngles(rotation);
    }

}