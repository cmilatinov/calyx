#pragma once

#include "math/Math.h"

namespace Calyx {

    class Transform {

    public:
        Transform();
        Transform(const vec3& position, const vec3& rotation, const vec3& scale);
        explicit Transform(const mat4& matrix);

        vec3 TransformPosition(const vec3& position) const;
        vec3 TransformDirection(const vec3& direction) const;

        vec3 InverseTransformPosition(const vec3& position) const;
        vec3 InverseTransformDirection(const vec3& direction) const;

        vec3 Forward() const;
        vec3 Left() const;
        vec3 Right() const;

        void Reset();
        void UpdateTransform() { return UpdateMatrix(); }

        void SetParent(Transform* parent) { m_parent = parent; }
        void SetPosition(const vec3& position);
        void SetRotation(const vec3& rotation);
        void SetScale(const vec3& scale);

        void SetWorldMatrix(const mat4& matrix);
        void SetLocalMatrix(const mat4& matrix);

        void Translate(const vec3& translation);
        void Rotate(const vec3& rotation);
        void Scale(const vec3& scale);

        vec3 GetPosition() const { return m_position; }
        vec3 GetRotation() const { return m_rotation; }
        vec3 GetScale() const { return m_scale; }
        mat4 GetLocalMatrix() const { return m_matrix; }

        float* GetPositionPtr() { return glm::value_ptr(m_position); }
        float* GetRotationPtr() { return glm::value_ptr(m_rotation); }
        float* GetScalePtr() { return glm::value_ptr(m_scale); }

        mat4 GetParentMatrix() const;

        mat4 GetMatrix() const;
        mat4 GetMatrix(Map<const Transform*, mat4>& cache) const;
        mat4 GetInverseMatrix() const;
        mat4 GetInverseMatrix(Map<const Transform*, mat4>& cache) const;

    private:
        Transform* m_parent = nullptr;
        vec3 m_position;
        vec3 m_rotation;
        vec3 m_scale;
        mat4 m_matrix;

        void UpdateMatrix();
        void UpdateComponents();

    };

}