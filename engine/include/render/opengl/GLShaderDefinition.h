#pragma once

#include "render/objects/ShaderDefinition.h"

namespace Calyx {

    class CALYX_API GLShaderDefinition : public ShaderDefinition {

    public:
        explicit GLShaderDefinition(const String& file);
        ~GLShaderDefinition() override;

        bool Load(const String& file) override;

    private:
        String m_name;
        String m_content;

    };

}