#pragma once

namespace Calyx {

    using AssetType = IdentifierType;

    class CALYX_API IAsset {
        CX_BEGIN_REFLECT();

    public:
        virtual ~IAsset() = default;

        virtual bool Load(const String& file) = 0;

        virtual AssetType GetAssetType() const = 0;

    };

    template<typename T>
    class CALYX_API Asset : public IAsset {

    public:
        AssetType GetAssetType() const override {
            return entt::resolve<T>().id();
        }

    };

}