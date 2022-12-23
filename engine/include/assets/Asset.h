#pragma once

namespace Calyx {

    using AssetT = IdentifierT;

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
            return entt::resolve<T>().id();
        }

    };

}