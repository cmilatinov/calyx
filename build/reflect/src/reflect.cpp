#include "reflect.h"

#include <iterator>
#include <string>
#include <unordered_map>

using namespace reflect;
using namespace std;

namespace {
    void NoOp(void*) {}

    unordered_map<int, IType*>& GetTypesMap() {
        static unordered_map<int, IType*> types;
        return types;
    }

    unordered_multimap<string, IFunction*>& GetFunctionsMap() {
        static unordered_multimap<string, IFunction*> functions;
        return functions;
    }

    unordered_multimap<string, IEnum*>& GetEnumsMap() {
        static unordered_multimap<string, IEnum*> enums;
        return enums;
    }

    unordered_multimap<string, IClass*>& GetClassesMap() {
        static unordered_multimap<string, IClass*> classes;
        return classes;
    }

    string NormalizeTypeName(const string& name) {
        const string ignored_prefix = "::";

        if (name.compare(0, ignored_prefix.size(), ignored_prefix) == 0) {
            return name.substr(ignored_prefix.size());
        } else {
            return name;
        }
    }
}

Object::Object()
        : id_(GetTypeId<void>()), deleter_(NoOp) {
}

Object::~Object() {
    deleter_(data_);
}

Object::Object(Object&& o)
        : deleter_(NoOp) {
    *this = move(o);
}

Object& Object::operator=(Object&& o) {
    // Release existing resource if any.
    deleter_(data_);

    id_ = o.id_;
    data_ = o.data_;
    deleter_ = move(o.deleter_);
    o.deleter_ = NoOp;
    return *this;
}

template<>
Object IFunction::operator()<>() {
    return this->Invoke({});
}

template<>
Object IMethod::operator()<>(const Reference& o) {
    return this->Invoke(o, {});
}

bool Object::IsVoid() const {
    return id_ == GetTypeId<void>();
}

int Object::GetTypeID() const {
    return id_;
}

Reference::Reference(int typeID, void* data)
        : id_(typeID), data_(data) {}

Reference::Reference(const Reference& o) = default;

Reference& Reference::operator=(const Reference& o) = default;

atomic<int> reflect::global_id;

IType* registry::GetTypeById(int typeID) {
    auto types = GetTypesMap();
    return types.find(typeID) != types.end() ? types[typeID] : nullptr;
}

vector<IType*> registry::GetTypesByName(const std::string& name) {
    vector<IType*> types;

    for (auto f: GetFunctionsByName(name))
        types.push_back(f);

    for (auto c: GetClassesByName(name))
        types.push_back(c);

    for (auto e: GetEnumsByName(name))
        types.push_back(e);

    return types;
}

vector<IFunction*> registry::GetFunctionsByName(const string& name) {
    string fnName = NormalizeTypeName(name);
    auto range = GetFunctionsMap().equal_range(fnName);

    vector<IFunction*> functions;
    functions.reserve(distance(range.first, range.second));
    for (auto it = range.first; it != range.second; ++it) {
        functions.push_back(it->second);
    }

    return functions;
}

vector<IClass*> registry::GetClassesByName(const string& name) {
    string clName = NormalizeTypeName(name);
    auto range = GetClassesMap().equal_range(clName);

    vector<IClass*> classes;
    classes.reserve(distance(range.first, range.second));
    for (auto it = range.first; it != range.second; ++it) {
        classes.push_back(it->second);
    }

    return classes;
}

vector<IEnum*> registry::GetEnumsByName(const string& name) {
    string enName = NormalizeTypeName(name);
    auto range = GetEnumsMap().equal_range(enName);

    vector<IEnum*> enums;
    enums.reserve(distance(range.first, range.second));
    for (auto it = range.first; it != range.second; ++it) {
        enums.push_back(it->second);
    }

    return enums;
}

void registry::internal::Register(IFunction* f) {
    GetFunctionsMap().insert(make_pair(f->GetName(), move(f)));
}

void registry::internal::Register(IClass* c) {
    GetClassesMap().insert(make_pair(c->GetName(), move(c)));
}

void registry::internal::Register(IEnum* e) {
    GetEnumsMap().insert(make_pair(e->GetName(), move(e)));
}

Exception::Exception(string error)
        : what_(move(error)) {}

const char* Exception::what() const noexcept {
    return what_.c_str();
}

Reference IClass::GetField(const Reference& o, const string& name) const {
    std::optional<Field> field = GetFieldByName(name);
    if (!field.has_value())
        throw new Exception("No such field!");

    if (field.value().storageType == StorageType::STATIC) {
        return Reference(
            field.value().typeID,
            reinterpret_cast<void*>(field.value().offset)
        );
    }

    return Reference(
        field.value().typeID,
        reinterpret_cast<void*>(
            reinterpret_cast<char*>(o.Ptr()) + field.value().offset
        )
    );
}

std::optional<Field> IClass::GetFieldByName(const string& name) const {
    std::vector<Field> fields = GetFields();
    auto field = std::find_if(fields.begin(), fields.end(), [&name](Field f) {{ return f.name == name; }});
    if (field != fields.end())
        return std::make_optional<Field>(*field);
    return std::nullopt;
}

bool IClass::HasField(const string& name) const {
    std::vector<Field> fields = GetFields();
    auto field = std::find_if(fields.begin(), fields.end(), [&name](Field f) {{ return f.name == name; }});
    return field != fields.end();
}

std::vector<IMethod*> IClass::GetMethodsByName(const string& name) const {
    auto methods = GetMethods();
    std::vector<IMethod*> result;
    std::copy_if(methods.begin(), methods.end(), std::back_inserter(result),
                 [&name](IMethod* m){{ return m->GetName() == name; }});
    return result;
}

IMethod* IClass::GetMethodByName(const string& name) const {
    auto methods = GetMethodsByName(name);
    return methods.size() > 0 ? methods[0] : nullptr;
}

IFunction* IClass::GetFunctionByName(const string& name) const {
    return GetMethodByName(name);
}

bool IClass::HasMethod(const string& name) const {
    auto methods = GetMethodsByName(name);
    return methods.size() > 0;
}