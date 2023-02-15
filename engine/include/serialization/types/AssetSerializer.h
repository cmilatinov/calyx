#pragma once

#include "assets/Asset.h"
#include "serialization/TypeSerializer.h"

namespace Calyx {

    class AssetSerializer : public TypeSerializer<Ref<IAsset>> {
        CX_BEGIN_REFLECT();
        void CX_ON_SERIALIZE(std::ostream* stream, const Ref<IAsset>& value);
        void CX_ON_DESERIALIZE(std::istream* stream, Ref<IAsset>& value);
    };

}