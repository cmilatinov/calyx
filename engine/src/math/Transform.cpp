#include "math/Transform.h"

namespace Calyx {

    Transform::Transform()
        : m_position(0, 0, 0),
          m_rotation(0, 0, 0),
          m_scale(1, 1, 1),
          m_matrix() {
        UpdateMatrix();
    }

    Transform::Transform(const vec3& position, const vec3& rotation, const vec3& scale)
        : m_position(position),
          m_rotation(rotation),
          m_scale(scale),
          m_matrix() {
        UpdateMatrix();
    }

    Transform::Transform(const mat4& matrix)
        : m_position(),
          m_rotation(),
          m_scale(),
          m_matrix(matrix) {
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

    vec3 Transform::Forward() const {
        return TransformDirection(vec3(0, 0, 1));
    }

    vec3 Transform::Left() const {
        return TransformDirection(vec3(-1, 0, 0));
    }

    vec3 Transform::Right() const {
        return TransformDirection(vec3(1, 0, 0));
    }

    void Transform::LookAt(const vec3& position) {
        auto diff = m_position - position;
        if (glm::length(diff) <= 0.00001f) return;
        quat rotation = glm::quatLookAt(glm::normalize(diff), vec3(0, 1, 0));
        m_rotation = glm::degrees(glm::eulerAngles(rotation));
        UpdateMatrix();
    }

    void Transform::Reset() {
        return SetWorldMatrix(glm::identity<mat4>());
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

    void Transform::SetWorldMatrix(const mat4& matrix) {
        if (m_parent == nullptr)
            return SetLocalMatrix(matrix);
        m_matrix = m_parent->GetInverseMatrix() * matrix;
        UpdateComponents();
    }

    void Transform::SetLocalMatrix(const mat4& matrix) {
        m_matrix = matrix;
        UpdateComponents();
    }

    void Transform::Translate(const vec3& translation) {
        m_position += translation;
        UpdateMatrix();
    }

    void Transform::Rotate(const vec3& rotation) {
        m_rotation += rotation;
        UpdateMatrix();
    }

    void Transform::Scale(const vec3& scale) {
        m_scale *= scale;
        UpdateMatrix();
    }

    mat4 Transform::GetParentMatrix() const {
        if (m_parent == nullptr)
            return glm::identity<mat4>();
        return m_parent->GetMatrix();
    }

    mat4 Transform::GetMatrix() const {
        if (m_parent == nullptr)
            return m_matrix;
        return m_parent->GetMatrix() * m_matrix;
    }

    mat4 Transform::GetMatrix(Map<const Transform*, mat4>& cache) const {
        // Root node in the transform hierarchy tree
        if (m_parent == nullptr) {
            cache[this] = m_matrix;
            return m_matrix;
        }

        // Check cache contains matrix
        CX_MAP_FIND(cache, this, entry) {
            return entry->second;
        }

        // Compute world transform recursively
        mat4 transform = m_parent->GetMatrix(cache) * m_matrix;
        cache[this] = transform;
        return transform;
    }

    mat4 Transform::GetInverseMatrix() const {
        if (m_parent == nullptr)
            return glm::inverse(m_matrix);
        return glm::inverse(m_matrix) * m_parent->GetInverseMatrix();
    }

    mat4 Transform::GetInverseMatrix(Map<const Transform*, mat4>& cache) const {
        // Root node in the transform hierarchy tree
        if (m_parent == nullptr) {
            mat4 inverseTransform = glm::inverse(m_matrix);
            cache[this] = inverseTransform;
            return inverseTransform;
        }

        // Check cache contains matrix
        CX_MAP_FIND(cache, this, entry) {
            return entry->second;
        }

        // Compute inverse world transform recursively
        mat4 inverseTransform = glm::inverse(m_matrix) * m_parent->GetInverseMatrix(cache);
        cache[this] = inverseTransform;
        return inverseTransform;
    }

    void Transform::UpdateMatrix() {
        m_matrix = Math::ComposeTransform(m_position, m_rotation, m_scale);
    }

    void Transform::UpdateComponents() {
        Math::DecomposeTransform(m_matrix, m_position, m_rotation, m_scale);
    }

    void Transform::CX_ON_POST_DESERIALIZE() {
        UpdateComponents();
    }

}