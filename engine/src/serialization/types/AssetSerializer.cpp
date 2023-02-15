#include "serialization/types/AssetSerializer.h"
#include "assets/AssetRegistry.h"
#include "serialization/Serializer.h"

namespace Calyx {

    void AssetSerializer::CX_ON_SERIALIZE(std::ostream* stream, const Ref<IAsset>& value) {
        auto id = AssetRegistry::GetAssetID(value);
        Serializer::DefaultSerialize(*stream, id);
    }

    void AssetSerializer::CX_ON_DESERIALIZE(std::istream* stream, Ref<IAsset>& value) {
        UUID id;
        Serializer::DefaultDeserialize(*stream, id);
        value = AssetRegistry::LoadAsset(id);
    }

}