#pragma once

#include <fstream>
#include <sstream>
#include <glad/glad.h>

#include "render/objects/Shader.h"

using std::ifstream;
using std::stringstream;
using std::filesystem::path;

namespace Calyx {

    class CALYX_API GLShader : public Shader {

    public:
        explicit GLShader(const String& filepath);
        ~GLShader() override;

        bool Load(const String& file) override;

        void Bind() const override;
        void Unbind() const override;

        void SetBool(const String& name, bool value) override;
        void SetInt(const String& name, int value) override;
        void SetIntArray(const String& name, uint32 count, int* values) override;
        void SetFloat(const String& name, float value) override;
        void SetFloat2(const String& name, const vec2& value) override;
        void SetFloat3(const String& name, const vec3& value) override;
        void SetFloat4(const String& name, const vec4& value) override;
        void SetMat4(const String& name, const mat4& value) override;

        const String& GetName() const override { return m_name; };
        void SetName(const String& name) override { m_name = name; };

        bool IsFunctional() const override;

    private:
        uint32 m_shaderProgramID = 0;
        String m_name;

        Map<GLenum, String> SplitShaderSrc(const String& shaderSrc);

        bool CreateShaderProgram(const Map<GLenum, String>& shaderSources);

        uint32 CreateShader(GLuint shaderType, const String& src);

    };

}