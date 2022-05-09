#pragma once

namespace Calyx {

    class Transform {

    public:
        Transform();
        Transform(const vec3& position, const vec3& rotation, const vec3& scale);
        Transform(const mat4& matrix);

        vec3 TransformPosition(const vec3& position) const;
        vec3 TransformDirection(const vec3& direction) const;

        vec3 InverseTransformPosition(const vec3& position) const;
        vec3 InverseTransformDirection(const vec3& direction) const;

        void SetParent(const Ref<Transform>& parent) { m_parent = parent; }
        void SetPosition(const vec3& position);
        void SetRotation(const vec3& rotation);
        void SetScale(const vec3& scale);
        void SetMatrix(const mat4& matrix);

        vec3 GetPosition() const { return m_position; }
        vec3 GetRotation() const { return m_rotation; }
        vec3 GetScale() const { return m_scale; }
        mat4 GetMatrix() const;
        mat4 GetInverseMatrix() const;

    private:
        Ref<Transform> m_parent = nullptr;
        vec3 m_position;
        vec3 m_rotation;
        vec3 m_scale;
        mat4 m_matrix;

        void UpdateMatrix();
        void UpdateComponents();

    };

}