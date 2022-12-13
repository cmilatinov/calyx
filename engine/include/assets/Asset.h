#pragma once

namespace Calyx {

    using AssetT = size_t;

    class IAsset {
        CX_BEGIN_REFLECT();

    public:
        virtual ~IAsset() = default;

        virtual AssetT GetAssetType() = 0;

    };

    template<typename T>
    class Asset : public IAsset {

    public:
        AssetT GetAssetType() override {
            return typeid(T).hash_code();
        }

    };

}