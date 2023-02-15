#pragma once

#include "serialization/TypeSerializer.h"
#include "extra/TypeTracker.h"

namespace Calyx {

    class Serializer : public TypeTracker<ITypeSerializer> {
    CX_SINGLETON(Serializer);

    public:
        Serializer();

        CX_SINGLETON_EXPOSE_METHOD(_Serialize, void Serialize(std::ostream& stream, const entt::meta_any& object), stream, object);

        template<typename T>
        static void DefaultSerialize(std::ostream& stream, const T& value) {
            stream.write(reinterpret_cast<const char*>(&value), sizeof(T));
        }

        template<typename T>
        static void DefaultDeserialize(std::istream& stream, T& value) {
            stream.read(reinterpret_cast<char*>(&value), sizeof(T));
        }

    private:
        void _Serialize(std::ostream& stream, const entt::meta_any& object);
        void SerializeDefault(std::ostream& stream, const entt::meta_any& object);

        void InvokeSerializeFunction(
            std::ostream& stream,
            const entt::meta_func& fn,
            entt::meta_any& serializer,
            const entt::meta_any& instance
        );

        void InvokeDeserializeFunction(
            std::istream& stream,
            const entt::meta_func& fn,
            entt::meta_any& serializer,
            entt::meta_any& instance
        );

    };

}