#pragma once

namespace Calyx {

    template<typename Base>
    class TypeTracker {
        static_assert(std::is_base_of_v<ITypeProcessor, Base>, "Base must extend ITypeProcessor!");

    protected:
        void InitializeTypes() {
            List<entt::meta_type> types = Reflect::Core::GetDerivedClasses<Base>();
            for (const auto& type: types) {
                auto instance = type.construct();
                if (!instance)
                    continue;

                auto instancePtr = instance.try_cast<Base>();
                if (instancePtr == nullptr)
                    continue;

                auto processedType = instancePtr->GetMetaType();
                if (Reflect::Core::IsRefType(processedType)) {
                    auto ptrType = Reflect::Core::GetRefPointerType(processedType);
                    RegisterRefTypeProcessor(ptrType, instance);
                } else {
                    RegisterTypeProcessor(processedType, instance);
                }
            }
        }

        bool ProcessorLookup(entt::id_type type, bool isRef, entt::meta_any** processor) {
            if (isRef) {
                CX_MAP_FIND(m_typeMap, type, i_type) {
                    type = i_type->second;
                }
            }

            CX_MAP_FIND(m_typeProcessors, type, i_instance) {
                *processor = &i_instance->second;
                return true;
            }

            return false;
        }

    protected:
        bool CheckProcessorExists(const entt::meta_any& instance, entt::meta_any** processor) {
            auto type = instance.type();
            auto id = type.id();

            // Check type is Ref<...>
            auto isRef = Reflect::Core::IsRefType(type);
            if (isRef) {
                id = Reflect::Core::GetRefPointerType(type).id();
            }

            return ProcessorLookup(id, isRef, processor);
        }

        bool CheckProcessorFunctionExists(
            entt::id_type function,
            const entt::meta_any& processor,
            entt::meta_func* fn
        ) {
            auto processorFn = processor.type().func(function);
            if (!processorFn)
                return false;

            if (fn != nullptr)
                *fn = processorFn;

            return true;
        }

        virtual void RegisterTypeProcessor(const entt::meta_type& type, const entt::meta_any& instance) {
            m_typeProcessors[type.id()] = instance;
        }

        virtual void RegisterRefTypeProcessor(const entt::meta_type& type, const entt::meta_any& instance) {
            m_typeProcessors[type.id()] = instance;
            List<entt::meta_type> derivedClasses = Reflect::Core::GetDerivedClasses(type);
            for (const auto& derived: derivedClasses) {
                m_typeMap[derived.id()] = type.id();
            }
        }

    protected:
        Map<entt::id_type, entt::id_type> m_typeMap;
        Map<entt::id_type, entt::meta_any> m_typeProcessors;

    };

}