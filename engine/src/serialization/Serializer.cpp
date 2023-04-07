#include "serialization/Serializer.h"
#include "ecs/Component.h"

namespace Calyx {

    CX_SINGLETON_INSTANCE(Serializer);

    Serializer::Serializer() {
        InitializeTypes();
    }

    void Serializer::_Serialize(std::ostream& stream, const entt::meta_any& object) {
        using namespace entt::literals;

        entt::meta_any* serializer;
        entt::meta_func fn;
        if (!CheckProcessorExists(object, &serializer) ||
            !CheckProcessorFunctionExists(CX_ON_SERIALIZE_HS, *serializer, &fn))
            return SerializeDefault(stream, object);

        InvokeSerializeFunction(stream, fn, *serializer, object);
    }

    void Serializer::SerializeDefault(std::ostream& stream, const entt::meta_any& object) {
        auto type = object.type();
        auto nDataAttributes = std::distance(type.data().begin(), type.data().end());
        if (type.is_arithmetic() ||
            (nDataAttributes <= 0 && Reflect::Core::HasTypeTrait(type, CX_TRAIT_TRIVIALLY_COPYABLE))) {
            stream.write((char*)object.data(), type.size_of());
            return;
        }

        for (const auto& [id, data]: type.data()) {
            Serialize(stream, data.get(object));
        }
    }

    void Serializer::_Deserialize(std::istream& stream, entt::meta_any& object) {
        using namespace entt::literals;

        entt::meta_any* serializer;
        entt::meta_func fn;
        if (!CheckProcessorExists(object, &serializer) ||
            !CheckProcessorFunctionExists(CX_ON_DESERIALIZE_HS, *serializer, &fn))
            return DeserializeDefault(stream, object);

        InvokeDeserializeFunction(stream, fn, *serializer, object);
    }

    void Serializer::DeserializeDefault(std::istream& stream, entt::meta_any& object) {
        using namespace entt::literals;

        auto type = object.type();
        auto nDataAttributes = std::distance(type.data().begin(), type.data().end());
        if (type.is_arithmetic() ||
            (nDataAttributes <= 0 && Reflect::Core::HasTypeTrait(type, CX_TRAIT_TRIVIALLY_COPYABLE))) {
            stream.read((char*)object.data(), type.size_of());
            return;
        }

        for (const auto& [id, data]: type.data()) {
            auto ref = Reflect::Core::CreateOpaqueReference(data.type(), Reflect::Core::GetFieldPointer(object, id));
            Deserialize(stream, ref);
        }

        entt::meta_func fn;
        if (CheckProcessorFunctionExists(CX_ON_POST_DESERIALIZE_HS, object, &fn)) {
            fn.invoke(object);
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

    void Serializer::InvokeDeserializeFinishedFunction(
        const entt::meta_func& fn,
        entt::meta_any& component
    ) {
        fn.invoke(component);
    }


}