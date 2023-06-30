#include "render/opengl/GLShaderDefinition.h"

#include <fstream>
#include <glad/glad.h>

namespace Calyx {

    GLShaderDefinition::GLShaderDefinition(const String& file) {
        GLShaderDefinition::Load(file);
    }

    GLShaderDefinition::~GLShaderDefinition() {
        glDeleteNamedStringARB(m_name.size(), m_name.c_str());
    }

    bool GLShaderDefinition::Load(const String& file) {
        Path filepath(file);
        std::ifstream stream(filepath);
        if (!stream.is_open()) return false;
        m_content = String(std::istreambuf_iterator<char>(stream), std::istreambuf_iterator<char>());
        m_name = "/" + filepath.stem().string();
        CX_CORE_TRACE("Loading shader definition '{}' ...", filepath.stem().string());
        glNamedStringARB(
            GL_SHADER_INCLUDE_ARB,
            m_name.size(), m_name.c_str(),
            m_content.size(), m_content.c_str()
        );
        return true;
    }

}