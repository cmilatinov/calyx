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

    class ShaderLibrary;

    class CALYX_API Shader : public Asset<Shader> {
        CX_ASSET_REGISTRY_FRIEND();
        friend class ShaderLibrary;

    public:
        ~Shader() override = default;

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

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

        static Scope<Shader> Create(const String& name, const String& vertexSrc, const String& fragmentSrc);

    private:
        static Shader* Create(const String& file);

    };

    class CALYX_API ShaderLibrary {

    public:
        void Add(const String& name, const Ref<Shader>& shader);
        void Add(const Ref<Shader>& shader);

        Ref<Shader> Load(const String& filepath);
        Ref<Shader> Load(const String& name, const String& filepath);

        Ref<Shader> Get(const String& name);

        bool Exists(const String& name) const;

    private:
        Map<String, Ref<Shader>> m_shaders;

    };

}

