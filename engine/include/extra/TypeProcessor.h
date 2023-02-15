#pragma once

namespace Calyx {

    class ITypeProcessor {

    public:
        virtual ~ITypeProcessor() = default;

        virtual const entt::meta_type& GetMetaType() const = 0;

    };

    template<typename Base, typename T>
    class TypeProcessor : public Base {
        static_assert(std::is_base_of_v<ITypeProcessor, Base>, "Base must extend ITypeProcessor!");

    public:
        TypeProcessor()
            : m_metaType(entt::resolve<T>()) {}

        const entt::meta_type& GetMetaType() const override { return m_metaType; }

    private:
        entt::meta_type m_metaType;

    };


}