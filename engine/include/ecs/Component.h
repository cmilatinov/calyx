#pragma once

namespace Calyx {

    class GameObject;

    class IComponent {

    public:
        virtual bool IsScriptable() const { return false; }
        virtual String GetName() const = 0;
        virtual size_t GetTypeId() const = 0;

    };

    template<typename T>
    class Component : public IComponent {

        friend class GameObject;

    public:
        String GetName() const override {
            return typeid(T).name();
        }

        size_t GetTypeId() const override {
            return typeid(T).hash_code();
        }

        template<typename C>
        C* GetComponent() const;

    protected:
        GameObject* m_gameObject = nullptr;

    };

    class IScript {
    public:
        virtual void Reset() {}
        virtual void Start() {}
        virtual void Update() {}
        virtual void DrawGizmos() {}
        virtual void Destroy() {}
    };

    template<typename T>
    class ScriptableComponent: public Component<T>, public IScript {

    public:
        bool IsScriptable() const { return true; }

    };

}