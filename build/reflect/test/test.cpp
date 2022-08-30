#include "test.gen.h"
//#include "test2.gen.h"

//#include <iostream>

using namespace reflect;
using namespace Calyx;

int main() {

    std::cout << Class<Fields>().GetFullName() << std::endl;

    std::cout << "Fields: " << std::endl;
    for (auto& field : Class<Fields>().GetFields()) {
        std::cout << field.typeName << " " <<  field.name << std::endl;
    }
    std::cout << std::endl;

    std::cout << Class<Methods>().GetFullName() << std::endl;
    std::cout << "Methods: " << std::endl;
    for (auto& method : Class<Methods>().GetMethods()) {
        std::cout << method->GetName() << std::endl;
    }
    std::cout << std::endl;

    Methods methods;
    IMethod* method = Class<Methods>().GetMethodByName("public_overload");
    std::cout << method->GetReturnType().typeName << std::endl;
    Object result = (*method)(methods, 5);
    std::cout << result.GetT<int>() << std::endl << std::endl;

    (*Class<Methods>().GetFunctionByName("public_static_void_void"))();
    std::cout << std::endl;

    (*Class<Methods>().GetMethodByName("private_void_void"))(methods);
    std::cout << std::endl;

    (*Class<Methods>().GetMethodByName("public_void_int"))(methods, 24);
    std::cout << std::endl;

//    std::cout << reflect::Enum<Calyx::Enum>().GetFullName() << std::endl;
//    std::cout << "Values: " << std::endl;
//    auto enumValues = reflect::Enum<Calyx::Enum>().GetEnumValues();
//    for (auto& it : enumValues) {
//        std::cout << it.first << " = " << it.second << std::endl;
//    }
//    int value;
//    std::string valueName;
//    reflect::Enum<Calyx::Enum>().Translate("ENUM5", value);
//    reflect::Enum<Calyx::Enum>().Translate(2, valueName);
//    std::cout << "ENUM5 -> " << value << std::endl;
//    std::cout << "2 -> " << valueName << std::endl << std::endl;
//
//    std::cout << reflect::Enum<AnotherEnum>().GetFullName() << std::endl;
//    std::cout << "Values: " << std::endl;
//    enumValues = reflect::Enum<AnotherEnum>().GetEnumValues();
//    for (auto& it : enumValues) {
//        std::cout << it.first << " = " << it.second << std::endl;
//    }
//    reflect::Enum<AnotherEnum>().Translate("Z", value);
//    reflect::Enum<AnotherEnum>().Translate(3, valueName);
//    std::cout << "Z -> " << value << std::endl;
//    std::cout << "3 -> " << valueName << std::endl << std::endl;
//
//    std::cout << "Helle, World!" << std::endl;

    return 0;
}