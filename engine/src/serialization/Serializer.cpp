#include "serialization/Serializer.h"

namespace Calyx {

    CX_SINGLETON_INSTANCE(Serializer);

    Serializer::Serializer() {
        InitializeTypes();
    }

    void Serializer::_Serialize(std::ostream& stream, const entt::meta_any& object) {
        using namespace entt::literals;

        entt::meta_any* serializer;
        if (!CheckProcessorExists(object, &serializer))
            return SerializeDefault(stream, object);

        entt::meta_func fn;
        if (!CheckProcessorFunctionExists(CX_ON_SERIALIZE_HS, *serializer, &fn))
            return;

        InvokeSerializeFunction(stream, fn, *serializer, object);
    }

    void Serializer::SerializeDefault(std::ostream& stream, const entt::meta_any& object) {
        auto type = object.type();
        if (type.is_arithmetic()) {
            CX_CORE_TRACE("{} - {}", type.info().name(), type.size_of());
//            stream.write((char*)object.data(), type.size_of());
            return;
        }

        for (const auto& [id, data]: type.data()) {
            Serialize(stream, data.get(object));
        }
    }

    void Serializer::InvokeSerializeFunction(
        std::ostream& stream,
        const entt::meta_func& fn,
        entt::meta_any& serializer,
        const entt::meta_any& instance
    ) {
        entt::meta_any args[2] = {
            &stream,
            instance.as_ref()
        };

        fn.invoke(
            serializer,
            (entt::meta_any* const)args,
            (const entt::meta_func::size_type)2
        );
    }

    void Serializer::InvokeDeserializeFunction(
        std::istream& stream,
        const entt::meta_func& fn,
        entt::meta_any& serializer,
        entt::meta_any& instance
    ) {
        auto type = instance.type();
        bool isRefType = Reflect::Core::IsRefType(type);
        entt::meta_any args[2] = {
            &stream,
            instance.as_ref()
        };

        if (isRefType) {
            args[1] = type.construct((entt::meta_any* const)&args[1], (const entt::meta_func::size_type)1);
        }

        fn.invoke(
            serializer,
            (entt::meta_any* const)args,
            (const entt::meta_func::size_type)2
        );

        if (isRefType) {
            const_cast<entt::meta_any&>(instance)
                .assign(type.construct((entt::meta_any* const)&args[1], (const entt::meta_func::size_type)1));
        }
    }

}