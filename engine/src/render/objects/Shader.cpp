#include "render/objects/Shader.h"
#include "render/Renderer.h"

#include "render/opengl/GLShader.h"

namespace Calyx {

    Scope<Shader> Shader::Create(const String& filepath) {
        switch (Renderer::GetAPI()) {
            case RendererAPI::OPENGL:
                return CreateScope<GLShader>(filepath);
        }
        CX_CORE_ASSERT(false, "Rendering API not supported!");
        return nullptr;
    }

    Scope<Shader> Shader::Create(const String& name, const String& vertexSrc, const String& fragmentSrc) {
        switch (Renderer::GetAPI()) {
            case RendererAPI::OPENGL:
                return CreateScope<GLShader>(name, vertexSrc, fragmentSrc);
        }
        CX_CORE_ASSERT(false, "Rendering API not supported!");
        return nullptr;
    }

    void ShaderLibrary::Add(const String& name, const Ref<Shader>& shader) {
        m_shaders[name] = shader;
    }

    void ShaderLibrary::Add(const Ref<Shader>& shader) {
        m_shaders[shader->GetName()] = shader;
    }

    Ref<Shader> ShaderLibrary::Load(const String& filepath) {
        Ref<Shader> shader = Shader::Create(filepath);
        Add(shader);
        return shader;
    }

    Ref<Shader> ShaderLibrary::Load(const String& name, const String& filepath) {
        Ref<Shader> shader = Shader::Create(filepath);
        shader->SetName(name);
        Add(name, shader);
        return shader;
    }

    Ref<Shader> ShaderLibrary::Get(const String& name) {
        return m_shaders[name];
    }

    bool ShaderLibrary::Exists(const String& name) const {
        return m_shaders.contains(name);
    }

}