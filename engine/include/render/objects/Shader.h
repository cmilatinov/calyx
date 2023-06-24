#pragma once

#include "assets/AssetRegistry.h"

namespace Calyx {

    enum class ShaderType {
        NONE,
        VERTEX,
        GEOMETRY,
        FRAGMENT
    };

    static ShaderType ShaderType(const String& str) {
        if (str == "vertex")
            return ShaderType::VERTEX;
        if (str == "geometry")
            return ShaderType::GEOMETRY;
        if (str == "fragment")
            return ShaderType::FRAGMENT;
        CX_CORE_ASSERT(false, "Invalid shader type!");
        return ShaderType::NONE;
    }

    class CALYX_API Shader : public Asset<Shader> {
        CX_BEGIN_REFLECT();
        CX_ASSET_REGISTRY_FRIEND();

    public:
        ~Shader() override = default;

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        virtual void SetBool(const String& name, bool value) = 0;
        virtual void SetInt(const String& name, int value) = 0;
        virtual void SetIntArray(const String& name, uint32 count, int* values) = 0;
        virtual void SetFloat(const String& name, float value) = 0;
        virtual void SetFloat2(const String& name, const vec2& value) = 0;
        virtual void SetFloat3(const String& name, const vec3& value) = 0;
        virtual void SetFloat4(const String& name, const vec4& value) = 0;
        virtual void SetMat4(const String& name, const mat4& value) = 0;

        virtual void SetName(const String& name) = 0;
        virtual const String& GetName() const = 0;
        virtual bool IsFunctional() const = 0;

    private:
        static Shader* Create(const String& file);

    };

}

