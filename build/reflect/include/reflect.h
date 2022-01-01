#pragma once

#include <string>
#include <vector>
#include <typeinfo>

using namespace std;

template<typename T>
struct TypeTag {};

struct Type {
    const string name;
    const string fullName;
    const size_t size;
};

struct Field {
    const Type* const type;
    const string name;
    const size_t offset;
};

template<int N>
struct Function {
    const string name;
    const Type* returnValue;
    const Field parameters[N];
};

struct Class : public Type {

};

template<typename T>
Type* const GetTypeImpl(TypeTag<T>);

template<typename T>
Type* const GetType() {
    return GetTypeImpl(TypeTag<T>{});
}

template<typename T>
Type* const GetTypeImpl(TypeTag<T>) {
    static Type type{
            string(typeid(T).name()),
            string(typeid(T).name()),
            sizeof(T)
    };
    return &type;
}

template<>
Type* const GetTypeImpl<string>(TypeTag<string>) {
    static Type type {
            string("string"),
            string("std::string"),
            sizeof(string)
    };
    return &type;
}

template<typename T>
Type* const GetTypeImpl(TypeTag<vector<T>>) {
    static Type type{
        string("vector"),
        string("std::vector"),
        sizeof(vector<T>)
    };
    return &type;
}