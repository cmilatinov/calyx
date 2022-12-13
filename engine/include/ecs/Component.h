#pragma once

namespace Calyx {

    class GameObject;

    class IComponent {
        CX_BEGIN_REFLECT();

    public:
        virtual ~IComponent() = default;

        virtual String GetName() const = 0;
        virtual TypeID GetTypeId() const = 0;

    };

    template<typename T>
    class Component : public IComponent {
        friend class GameObject;

    public:
        String GetName() const override {
            const char* name = NAMEOF_SHORT_TYPE(T).data();
            return name;
        }

        TypeID GetTypeId() const override {
            auto type = entt::resolve<T>();
            if (!type) {
                CX_CORE_WARN(
                    "Script '{}' is not registered as reflected, its hook methods will not work correctly.\n"
                    "Make sure to run the reflection tool on its header file.", NAMEOF_SHORT_TYPE(T).data()
                );
                entt::meta<T>().type(entt::hashed_string(NAMEOF_SHORT_TYPE(T).data()));
                type = entt::resolve<T>();
            }
            return type.id();
        }

        template<typename C>
        C* GetComponent() const;

    protected:
        GameObject* m_gameObject = nullptr;

    };

}