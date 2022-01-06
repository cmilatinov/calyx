#pragma once

#include <atomic>
#include <functional>
#include <optional>
#include <iterator>
#include <memory>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <vector>

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

        Reference(const Reference& o);

        Reference& operator=(const Reference& o);

        template<typename T>
        bool IsT() const;

        template<typename T>
        T& GetT() const;

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
        CLASS,
        ENUM
    };

    class IType {
    public:
        virtual ~IType() = default;

        virtual const std::string& GetName() const = 0;
        virtual const std::string& GetFullName() const = 0;
        virtual int GetID() const = 0;

        virtual TypeClass GetTypeClass() const { return TypeClass::PRIMITIVE; }
    };

    struct Parameter {
        IType* type;
        std::string name;
    };

    class IFunction : public IType {
    public:
        virtual IType* GetReturnType() const = 0;
        virtual std::vector<Parameter> GetParameterList() const = 0;

        // Syntactic sugar for calling Invoke().
        template<typename... Ts>
        Object operator()(Ts&& ... ts);

        virtual Object Invoke(const std::vector<Object>& args) = 0;

        virtual TypeClass GetTypeClass() const { return TypeClass::FUNCTION; }
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

        // Syntactic sugar for calling Invoke().
        template<typename... Ts>
        Object operator()(const Reference& o, Ts&& ... ts);

        virtual Object Invoke(const Reference& o, const std::vector<Object>& args) = 0;
    };

    struct Field {
        AccessSpecifier accessSpecifier;
        StorageType storageType;
        const IType& type;
        std::string name;
    };

    class IClass : public IType {
    public:
        virtual std::vector<Field> GetFields() const = 0;
        virtual std::optional<Field> GetFieldByName(const std::string& name) const = 0;
        virtual Reference GetField(const Reference& o, const std::string& name) const = 0;
        virtual bool HasField(const std::string& name) const = 0;

        virtual std::vector<const IMethod&> GetMethods() const = 0;
        virtual std::vector<const IMethod&> GetMethodsByName(const std::string& name) const = 0;
        virtual std::optional<const IMethod&> GetMethodByName(const std::string& name) const = 0;
        virtual bool HasMethod(const std::string& name) const = 0;

        virtual TypeClass GetTypeClass() const { return TypeClass::CLASS; }
    };

    class IEnum : public IType {
    public:
        virtual std::vector<std::string> GetStringValues() const = 0;
        virtual std::vector<int> GetIntValues() const = 0;

        virtual bool TryTranslate(const std::string& value, int& out) const = 0;
        virtual bool TryTranslate(int value, std::string& out) const = 0;

        virtual TypeClass GetTypeClass() const { return TypeClass::ENUM; }
    };

    namespace registry {
        std::vector<IType*> GetTypesByName(const std::string& name);
        std::vector<IFunction*> GetFunctionsByName(const std::string& name);
        std::vector<IClass*> GetClassesByName(const std::string& name);
        std::vector<IEnum*> GetEnumsByName(const std::string& name);

        namespace internal {
            void Register(std::unique_ptr<IFunction>&& f);
            void Register(std::unique_ptr<IClass>&& c);
            void Register(std::unique_ptr<IEnum>&& e);
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
    std::vector<Object> v({
        Object(std::forward<Ts>(ts))...
    });
    return this->Invoke(o, v);
}

template<typename... Ts>
reflect::Object reflect::IFunction::operator()(Ts&& ... ts) {
    std::vector<Object> v({
        Object(std::forward<Ts>(ts))...
    });
    return this->Invoke(v);
}
