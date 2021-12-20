#pragma once

#include <fstream>
#include <sstream>
#include <glad/glad.h>

#include "render/objects/Shader.h"

using std::ifstream;
using std::stringstream;
using std::filesystem::path;

namespace Calyx {

    class GLShader : public Shader {

    public:
        GLShader(const String& name, const String& vertexSrc, const String& fragmentSrc);
        GLShader(const String& name, const String& vertexSrc, const String& geometrySrc, const String& fragmentSrc);
        GLShader(const String& filepath);
        ~GLShader() override;

        void Bind() const override;
        void Unbind() const override;

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
        uint32 m_shaderProgramID;
        List<uint32> m_shaderIDs;
        String m_name;

        void LoadShader(const String& filepath);
        void LoadShader(const String& vertexSrc, const String& fragmentSrc);
        void LoadShader(const String& vertexSrc, const String& geometrySrc, const String& fragmentSrc);

        String LoadShaderSrc(const String& file);
        Map<GLenum, String> SplitShaderSrc(const String& shaderSrc);

        void CreateShaderProgram(const Map<GLenum, String>& shaderSources);

        uint32 CreateShader(GLuint shaderType, const String& src);

    };

}