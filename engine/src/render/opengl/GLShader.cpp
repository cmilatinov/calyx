#include "render/opengl/GLShader.h"
#include "render/opengl/GLUtils.h"

namespace Calyx {

    GLShader::GLShader(const String& name, const String& vertexSrc, const String& fragmentSrc) {
        m_name = name;
        LoadShader(vertexSrc, fragmentSrc);
    }

    GLShader::GLShader(
        const String& name, const String& vertexSrc, const String& geometrySrc,
        const String& fragmentSrc
    ) {
        m_name = name;
        LoadShader(vertexSrc, geometrySrc, fragmentSrc);
    }

    GLShader::GLShader(const String& filepath) {
        m_name = path(filepath).stem().string();
        LoadShader(filepath);
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

    void GLShader::SetInt(const String& name, int value) {
        glUniform1i(glGetUniformLocation(m_shaderProgramID, name.c_str()), value);
    }

    void GLShader::SetIntArray(const String& name, uint32 count, int* values) {
        glUniform1iv(glGetUniformLocation(m_shaderProgramID, name.c_str()), count, values);
    }

    void GLShader::SetFloat(const String& name, float value) {
        glUniform1f(glGetUniformLocation(m_shaderProgramID, name.c_str()), value);
    }

    void GLShader::SetFloat2(const String& name, const vec2& value) {
        glUniform2f(glGetUniformLocation(m_shaderProgramID, name.c_str()), value.x, value.y);
    }

    void GLShader::SetFloat3(const String& name, const vec3& value) {
        glUniform3f(glGetUniformLocation(m_shaderProgramID, name.c_str()), value.x, value.y, value.z);
    }

    void GLShader::SetFloat4(const String& name, const vec4& value) {
        glUniform4f(glGetUniformLocation(m_shaderProgramID, name.c_str()), value.x, value.y, value.z, value.w);
    }

    void GLShader::SetMat4(const String& name, const mat4& value) {
        glUniformMatrix4fv(
            glGetUniformLocation(m_shaderProgramID, name.c_str()), 1, GL_FALSE,
            reinterpret_cast<const GLfloat*>(&value));
    }

    bool GLShader::IsFunctional() const {
        return false;
    }

    void GLShader::LoadShader(const String& filepath) {
        String src = LoadShaderSrc(filepath);
        Map<GLenum, String> shaderSources = SplitShaderSrc(src);
        CreateShaderProgram(shaderSources);
    }

    void GLShader::LoadShader(const String& vertexSrc, const String& fragmentSrc) {
        Map<GLenum, String> shaderSources = {
            { GL_VERTEX_SHADER, vertexSrc },
            { GL_FRAGMENT_SHADER, fragmentSrc }
        };
        CreateShaderProgram(shaderSources);
    }

    void GLShader::LoadShader(const String& vertexSrc, const String& geometrySrc, const String& fragmentSrc) {
        Map<GLenum, String> shaderSources = {
            { GL_VERTEX_SHADER, vertexSrc },
            { GL_GEOMETRY_SHADER, geometrySrc },
            { GL_FRAGMENT_SHADER, fragmentSrc }
        };
        CreateShaderProgram(shaderSources);
    }

    Map<GLenum, String> GLShader::SplitShaderSrc(const String& source) {
        Map<GLenum, std::string> shaderSources;
        const char* typeToken = "#type";
        size_t typeTokenLength = strlen(typeToken);
        size_t pos = source.find(typeToken, 0); // Start of shader type declaration line

        while (pos != String::npos) {
            size_t eol = source.find_first_of("\r\n", pos); // End of shader type declaration line
            CX_CORE_ASSERT(eol != String::npos, "Syntax error");

            size_t begin = pos + typeTokenLength + 1; // Start of shader type name (after "#type " keyword)
            String type = source.substr(begin, eol - begin);
            GLenum shaderType = GLShaderType(type);
            CX_CORE_ASSERT(shaderType, "Invalid shader type specified");

            size_t nextLinePos = source.find_first_not_of(
                "\r\n",
                eol
            ); // Start of shader code after shader type declaration line
            CX_CORE_ASSERT(nextLinePos != String::npos, "Syntax error");

            pos = source.find(typeToken, nextLinePos); // Start of next shader type declaration line
            shaderSources[shaderType] = (pos == String::npos) ? source.substr(nextLinePos) : source.substr(
                nextLinePos,
                pos -
                nextLinePos
            );
        }

        return shaderSources;
    }

    String GLShader::LoadShaderSrc(const String& file) {
        ifstream fs(file.c_str());
        bool open = fs.is_open();
        CX_CORE_ASSERT(open, "Could not read shader file!");
        stringstream ss;
        ss << fs.rdbuf();
        return ss.str();
    }

    void GLShader::CreateShaderProgram(const Map<GLenum, String>& shaderSources) {
        // Create program
        m_shaderProgramID = glCreateProgram();

        // Check num shaders is 2 or 3
        int numShaders = shaderSources.size();
        CX_CORE_ASSERT(numShaders >= 2 && numShaders <= 3, "Invalid number of shader sources!");

        // Create shaders
        for (auto& entry: shaderSources) {
            uint32 shader = CreateShader(entry.first, entry.second);
            glAttachShader(m_shaderProgramID, shader);
        }

        // Link & check program
        int success;
        glLinkProgram(m_shaderProgramID);
        glGetProgramiv(m_shaderProgramID, GL_LINK_STATUS, &success);
        CX_CORE_ASSERT(success, "Shader program linkage failed!");
        if (!success) {
            char infoLog[512];
            glGetProgramInfoLog(m_shaderProgramID, sizeof(infoLog), nullptr, infoLog);
            CX_CORE_ERROR("Shader Linkage Error:");
            CX_CORE_ERROR(infoLog);
        }

        // Delete shaders
        for (auto shader: m_shaderIDs)
            glDeleteShader(shader);
    }

    uint32 GLShader::CreateShader(GLuint shaderType, const String& src) {
        const char* shaderSrc = src.c_str();
        uint32 shaderID = glCreateShader(shaderType);
        glShaderSource(shaderID, 1, &shaderSrc, nullptr);
        glCompileShader(shaderID);

        int success;
        glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
        CX_CORE_ASSERT(success, "Shader compilation failed!");
        if (!success) {
            char infoLog[512];
            glGetShaderInfoLog(shaderID, sizeof(infoLog), nullptr, infoLog);
            CX_CORE_ERROR("Shader Compilation Error:");
            CX_CORE_ERROR(infoLog);
        }

        m_shaderIDs.push_back(shaderID);

        return shaderID;
    }

}