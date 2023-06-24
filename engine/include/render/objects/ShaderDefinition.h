#pragma once

#include "assets/Asset.h"
#include "assets/AssetRegistry.h"

namespace Calyx {

    class CALYX_API ShaderDefinition : public Asset<ShaderDefinition> {
        CX_BEGIN_REFLECT();
        CX_ASSET_REGISTRY_FRIEND();

    public:
        ~ShaderDefinition() override = default;

    private:
        static ShaderDefinition* Create(const String& file);

    };

}