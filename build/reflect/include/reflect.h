#pragma once

#include <algorithm>
#include <atomic>
#include <functional>
#include <optional>
#include <iterator>
#include <memory>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <vector>
#include <unordered_map>

#define CX_PROPERTY(...)
#define CX_REFLECT_ENUM(...)

#define CX_BEGIN_REFLECT()      \
template<typename T>            \
friend class reflect::Class;    \
template<typename T, T t>       \
friend class reflect::Method;

namespace reflect {

    // Object is a class similar in nature to std::any, only it does not require
    // C++17 and does not require stored objects to be copyable.
    class Object final {
    public:
        // Constructs an Object with stored type of void.
        Object();

        // Constructs an Object with a copy of T.
        template<typename T>
        explicit Object(T&& t);

        // Constructs an Object with a reference to T.
        template<typename T>
        explicit Object(T& t);

        Object(Object&& o);

        Object& operator=(Object&& o);

        Object(const Object& o) = delete;

        Object& operator=(const Object& o) = delete;

        ~Object();

        template<typename T>
        bool IsT() const;

        template<typename T>
        const T& GetT() const;

        bool IsVoid() const;

        int GetTypeID() const;

    private:
        int id_;
        void* data_ = nullptr;
        std::function<void(void*)> deleter_;
    };

    // Reference is a non-const, type erased wrapper around any object.
    class Reference final {
    public:
        // Constructs a Reference to t.
        template<typename T>
        Reference(T& t);

        Reference(int typeID, void* data);

        Reference(const Reference& o);

        Reference& operator=(const Reference& o);

        template<typename T>
        bool IsT() const;

        template<typename T>
        T& GetT() const;

        void* Ptr() const { return data_; }

    private:
        int id_;
        void* data_ = nullptr;
    };

    extern std::atomic<int> global_id;

    template<typename T>
    int GetTypeId();

    enum class TypeClass {
        PRIMITIVE,
        FUNCTION,
        METHOD,
        CLASS,
        ENUM
    };

    class IType {
    public:
        virtual ~IType() = default;

        virtual std::string GetName() const = 0;
        virtual std::string GetFullName() const = 0;
        virtual int GetTypeID() const = 0;

        virtual TypeClass GetTypeClass() const = 0;
    };

    struct Parameter {
        int typeID;
        std::string typeName;
        std::string name;
    };

    struct ReturnType {
        int typeID;
        std::string typeName;
    };

    class IFunction : public IType {
    public:
        virtual ReturnType GetReturnType() const = 0;
        virtual std::vector<Parameter> GetParameterList() const = 0;

        template<typename... Ts>
        Object operator()(Ts&& ... ts);

        virtual Object Invoke(const std::vector<Object>& args) const = 0;

        TypeClass GetTypeClass() const override { return TypeClass::FUNCTION; }
    };

    enum class AccessSpecifier {
        PRIVATE,
        PROTECTED,
        PUBLIC
    };

    enum class StorageType {
        INSTANCE,
        STATIC
    };

    class IMethod : public IFunction {
    public:
        virtual AccessSpecifier GetAccessSpecifier() const = 0;
        virtual StorageType GetStorageType() const = 0;

        template<typename... Ts>
        Object operator()(const Reference& o, Ts&& ... ts);

        Object Invoke(const std::vector<Object>& args) const override = 0;
        virtual Object Invoke(const Reference& o, const std::vector<Object>& args) const = 0;

        TypeClass GetTypeClass() const override { return TypeClass::METHOD; }
    };

    struct Annotation {
        std::string name;
        std::vector<std::string> arguments;
    };

    struct Field {
        size_t offset;
        AccessSpecifier accessSpecifier;
        StorageType storageType;
        int typeID;
        std::string typeName;
        std::string name;
        std::vector<Annotation> annotations;
    };

    class IClass : public IType {
    public:
        virtual std::vector<Field> GetFields() const = 0;
        virtual std::vector<IMethod*> GetMethods() const = 0;

        Reference GetField(const Reference& o, const std::string& name) const;
        std::optional<Field> GetFieldByName(const std::string& name) const;
        bool HasField(const std::string& name) const;

        std::vector<IMethod*> GetMethodsByName(const std::string& name) const;
        IMethod* GetMethodByName(const std::string& name) const;
        IFunction* GetFunctionByName(const std::string& name) const;
        bool HasMethod(const std::string& name) const;

        TypeClass GetTypeClass() const override { return TypeClass::CLASS; }
    };

    class IEnum : public IType {
    public:
        virtual std::unordered_map<std::string, int> GetEnumValues() const = 0;

        virtual std::vector<std::string> GetStringValues() const = 0;
        virtual std::vector<int> GetIntValues() const = 0;

        virtual bool Translate(const std::string& value, int& out) const = 0;
        virtual bool Translate(int value, std::string& out) const = 0;

        TypeClass GetTypeClass() const override { return TypeClass::ENUM; }
    };

    namespace registry {
        IType* GetTypeById(int typeID);
        std::vector<IType*> GetTypesByName(const std::string& name);
        std::vector<IFunction*> GetFunctionsByName(const std::string& name);
        std::vector<IClass*> GetClassesByName(const std::string& name);
        std::vector<IEnum*> GetEnumsByName(const std::string& name);

        namespace internal {
            void Register(IFunction* f);
            void Register(IClass* c);
            void Register(IEnum* e);
        }
    };

    class Exception : virtual public std::exception {
    public:
        Exception(std::string error);

        const char* what() const noexcept override;

    private:
        std::string what_;
    };

    // The following classes are specialized with reflection metadata and can
    // be used directly instead of going through the registry.
    template<typename T>
    class Type : public IType {

    public:
        static std::unique_ptr<Type<T>> s_instance;

        virtual std::string GetName() const { return typeid(T).name(); }
        virtual std::string GetFullName() const { return typeid(T).name(); }
        virtual int GetID() const { return GetTypeId<T>(); }

        virtual TypeClass GetTypeClass() const { return TypeClass::PRIMITIVE; };

    };

    template<typename T>
    std::unique_ptr<Type<T>> Type<T>::s_instance = std::make_unique<Type<T>>();

    template<typename T>
    class Enum;

    template<typename T>
    class Class;

    template<typename T, T t>
    class Function;

    template<typename T, T t>
    class Method;

    template<>
    Object IFunction::operator()<>();

    template<>
    Object IMethod::operator()<>(const Reference& o);
}

// Implementation and specializations
template<typename T>
int reflect::GetTypeId() {
    // While it may seem like global_id could be an int rather than
    // std::atomic, this is in fact no true. GetTypeId<T1> does not
    // sync access with GetTypeId<T2>.
    static const int t_id = global_id++;
    return t_id;
}

template<typename T>
reflect::Object::Object(T&& t)
    : id_(GetTypeId<std::decay_t<T>>()),
      data_(new std::decay_t<T>(std::forward<T>(t))) {
    // This is not part of the initializer list because it
    // doesn't compile on VC.
    deleter_ = [](void* data) {
        delete static_cast<std::decay_t<T>*>(data);
    };
}

template<typename T>
reflect::Object::Object(T& t)
        : id_(GetTypeId<std::decay_t<T>>()),
          data_((void*)&t) {
    // This is not part of the initializer list because it
    // doesn't compile on VC.
    deleter_ = [](void* data) {};
}

template<typename T>
bool reflect::Object::IsT() const {
    return GetTypeId<T>() == id_;
}

template<typename T>
const T& reflect::Object::GetT() const {
    if (GetTypeId<T>() != id_)
        throw Exception("GetT() failed: incompatible T.");
    return *static_cast<T*>(data_);
}

template<typename T>
reflect::Reference::Reference(T& t)
    : id_(GetTypeId<std::remove_reference_t<T>>()), data_((void*) &t) {}

template<typename T>
bool reflect::Reference::IsT() const {
    return GetTypeId<T>() == id_;
}

template<typename T>
T& reflect::Reference::GetT() const {
    if (GetTypeId<T>() != id_)
        throw Exception("GetT() failed: incompatible T.");
    return *static_cast<T*>(data_);
}

template<typename... Ts>
reflect::Object reflect::IMethod::operator()(const Reference& o, Ts&& ... ts) {
    Object init[] = { Object(std::forward<Ts>(ts))... };
    std::vector<Object> v(
            std::make_move_iterator(std::begin(init)),
            std::make_move_iterator(std::end(init)));
    return this->Invoke(o, std::move(v));
}

template<typename... Ts>
reflect::Object reflect::IFunction::operator()(Ts&& ... ts) {
    Object init[] = { Object(std::forward<Ts>(ts))... };
    std::vector<Object> v(
            std::make_move_iterator(std::begin(init)),
            std::make_move_iterator(std::end(init)));
    return this->Invoke(std::move(v));
}