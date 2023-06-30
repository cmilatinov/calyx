#include "render/opengl/GLShader.h"
#include "render/opengl/GLUtils.h"

namespace Calyx {

    GLShader::GLShader(const String& filepath) {
        GLShader::Load(filepath);
    }

    GLShader::~GLShader() {
        glDeleteProgram(m_shaderProgramID);
    }

    void GLShader::Bind() const {
        glUseProgram(m_shaderProgramID);
    }

    void GLShader::Unbind() const {
        glUseProgram(0);
    }

    void GLShader::SetBool(const String& name, bool value) {
        glUniform1i(glGetUniformLocation(m_shaderProgramID, name.c_str()), value ? GL_TRUE : GL_FALSE);
    }

    void GLShader::SetInt(const String& name, int value) {
        glUniform1i(glGetUniformLocation(m_shaderProgramID, name.c_str()), value);
    }

    void GLShader::SetIntArray(const String& name, uint32 count, int* values) {
        glUniform1iv(glGetUniformLocation(m_shaderProgramID, name.c_str()), count, values);
    }

    void GLShader::SetFloat(const String& name, float value) {
        glUniform1f(
            glGetUniformLocation(m_shaderProgramID, name.c_str()),
            value
        );
    }

    void GLShader::SetFloat2(const String& name, const vec2& value) {
        glUniform2f(
            glGetUniformLocation(m_shaderProgramID, name.c_str()),
            value.x, value.y
        );
    }

    void GLShader::SetFloat3(const String& name, const vec3& value) {
        glUniform3f(
            glGetUniformLocation(m_shaderProgramID, name.c_str()),
            value.x, value.y, value.z
        );
    }

    void GLShader::SetFloat4(const String& name, const vec4& value) {
        glUniform4f(
            glGetUniformLocation(m_shaderProgramID, name.c_str()),
            value.x, value.y, value.z, value.w
        );
    }

    void GLShader::SetMat4(const String& name, const mat4& value) {
        glUniformMatrix4fv(
            glGetUniformLocation(m_shaderProgramID, name.c_str()),
            1, GL_FALSE, reinterpret_cast<const GLfloat*>(&value)
        );
    }

    bool GLShader::IsFunctional() const {
        if (!m_shaderProgramID) return false;
        int success;
        glGetProgramiv(m_shaderProgramID, GL_LINK_STATUS, &success);
        return success;
    }

    bool GLShader::Load(const String& filepath) {
        m_name = Path(filepath).stem().string();
        String src = FileUtils::ReadEntireFile(filepath);
        if (src.empty()) return false;
        Map<GLenum, String> shaderSources = SplitShaderSrc(src);
        return CreateShaderProgram(shaderSources);
    }

    Map<GLenum, String> GLShader::SplitShaderSrc(const String& source) {
        Map<GLenum, std::string> shaderSources;
        const char* typeToken = "#pragma shader";
        size_t typeTokenLength = strlen(typeToken);
        size_t pos = source.find(typeToken, 0);

        while (pos != String::npos) {
            size_t eol = source.find_first_of("\r\n", pos);
            CX_CORE_ASSERT(eol != String::npos, "Syntax error");

            size_t begin = pos + typeTokenLength + 1;
            String type = source.substr(begin, eol - begin);
            GLenum shaderType = GLShaderType(type);
            CX_CORE_ASSERT(shaderType, "Invalid shader type specified");

            size_t nextLinePos = source.find_first_not_of("\r\n", eol);
            CX_CORE_ASSERT(nextLinePos != String::npos, "Syntax error");

            pos = source.find(typeToken, nextLinePos);
            shaderSources[shaderType] = (pos == String::npos) ? source.substr(nextLinePos) : source.substr(
                nextLinePos,
                pos -
                nextLinePos
            );
        }

        return shaderSources;
    }

    bool GLShader::CreateShaderProgram(const Map<GLenum, String>& shaderSources) {
        CX_CORE_TRACE("Compiling shader '{}' ...", m_name);

        // Create program
        if (!m_shaderProgramID) {
            m_shaderProgramID = glCreateProgram();
        }

        // Check num shaders is 2 or 3
        int numShaders = shaderSources.size();
        CX_CORE_ASSERT(numShaders >= 2 && numShaders <= 3, "Invalid number of shader sources!");

        // Create shaders
        List<uint32> shaders;
        shaders.reserve(shaderSources.size());
        for (auto& entry: shaderSources) {
            uint32 shader = CreateShader(entry.first, entry.second);
            if (!shader) return false;
            glAttachShader(m_shaderProgramID, shader);
            shaders.push_back(shader);
        }

        // Link & check program
        int success;
        glLinkProgram(m_shaderProgramID);
        glGetProgramiv(m_shaderProgramID, GL_LINK_STATUS, &success);
        if (!success) {
            char infoLog[512];
            glGetProgramInfoLog(m_shaderProgramID, sizeof(infoLog), nullptr, infoLog);
            CX_CORE_ERROR("Shader Linkage Error:");
            CX_CORE_ERROR(infoLog);
            return false;
        }

        // Delete shaders
        for (auto shader: shaders) {
            glDeleteShader(shader);
        }

        return true;
    }

    uint32 GLShader::CreateShader(GLuint shaderType, const String& src) {
        const char* shaderSrc = src.c_str();
        uint32 shaderID = glCreateShader(shaderType);
        glShaderSource(shaderID, 1, &shaderSrc, nullptr);
        static const char* path[] = { "/" };
        glCompileShaderIncludeARB(shaderID, 1, path, nullptr);

        int success;
        glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
        if (!success) {
            char infoLog[512];
            glGetShaderInfoLog(shaderID, sizeof(infoLog), nullptr, infoLog);
            CX_CORE_ERROR("Shader Compilation Error:");
            CX_CORE_ERROR(infoLog);
            return 0;
        }

        return shaderID;
    }

}