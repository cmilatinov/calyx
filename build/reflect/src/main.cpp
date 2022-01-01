#include "reflect.h"

#include <iostream>

int main() {

    Type* const type = GetType<int>();
    std::cout << type->name << std::endl;
    std::cout << type->fullName << std::endl;
    std::cout << type->size << std::endl;

    Type* const type1 = GetType<wchar_t>();
    std::cout << type1->name << std::endl;
    std::cout << type1->fullName << std::endl;
    std::cout << type1->size << std::endl;

    Type* const type2 = GetType<std::vector<int>>();
    std::cout << type2->name << std::endl;
    std::cout << type2->fullName << std::endl;
    std::cout << type2->size << std::endl;

    Type* const type3 = GetType<std::string>();
    std::cout << type3->name << std::endl;
    std::cout << type3->fullName << std::endl;
    std::cout << type3->size << std::endl;

    return 0;
}