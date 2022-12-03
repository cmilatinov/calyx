#pragma once

#include <glad/glad.h>

#include "render/objects/Texture.h"
#include "render/objects/Shader.h"
#include "render/objects/Query.h"

namespace Calyx {

    static GLenum GLShaderType(const String& str) {
        switch (ShaderType(str)) {
            case ShaderType::VERTEX:
                return GL_VERTEX_SHADER;
            case ShaderType::GEOMETRY:
                return GL_GEOMETRY_SHADER;
            case ShaderType::FRAGMENT:
                return GL_FRAGMENT_SHADER;
            default:
                CX_CORE_ASSERT(false, "Invalid shader type!");
                return 0;
        }
    }

    static GLenum GLInternalTextureFormat(TextureFormat format) {
        switch (format) {
            case TextureFormat::RGBA8:
                return GL_RGBA8;
            case TextureFormat::DEPTH32:
                return GL_DEPTH_COMPONENT32;
            case TextureFormat::DEPTH24_STENCIL8:
                return GL_DEPTH24_STENCIL8;
            default:
                CX_CORE_ASSERT(false, "Invalid texture format!");
                return 0;
        }
    }

    static GLenum GLTextureFormat(TextureFormat format) {
        switch (format) {
            case TextureFormat::RGBA8:
                return GL_RGBA;
            case TextureFormat::DEPTH32:
                return GL_DEPTH_COMPONENT;
            case TextureFormat::DEPTH24_STENCIL8:
                return GL_DEPTH_STENCIL;
            default:
                CX_CORE_ASSERT(false, "Invalid texture format!");
                return 0;
        }
    }

    static GLenum GLColorAttachment(uint32 attachmentIndex) {
        int maxColorAttachments;
        glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &maxColorAttachments);
        CX_CORE_ASSERT(attachmentIndex < maxColorAttachments, "Invalid attachment index!");
        return GL_COLOR_ATTACHMENT0 + attachmentIndex;
    }

    static bool IsDepthFormat(TextureFormat format) {
        switch (format) {
            case TextureFormat::RGBA8:
                return false;
            case TextureFormat::DEPTH32:
            case TextureFormat::DEPTH24_STENCIL8:
                return true;
            default:
                CX_CORE_ASSERT(false, "Invalid texture format!");
                return false;
        }
    }

    static GLenum GLQueryType(QueryType type) {
        switch (type) {
            case QueryType::PRIMITIVES_GENERATED:
                return GL_PRIMITIVES_GENERATED;
            case QueryType::TIME_ELAPSED:
                return GL_TIME_ELAPSED;
            default:
                CX_CORE_ASSERT(false, "Invalid query type!");
                return 0;
        }
    }

}