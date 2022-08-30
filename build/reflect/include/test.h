#pragma once

#include <string>
#include <iostream>

#include "reflect.h"

class Test {
    CX_BEGIN_REFLECT()
};

namespace Calyx {

    class Base1 {};

    CX_PROPERTY(Cool, Beans)
    class Base {
        CX_BEGIN_REFLECT()

        class Inner {
            CX_BEGIN_REFLECT()

        protected:
            std::string pog;
        };

    public:
        int base;
        int another_one;
        static const int yay = 123;
    };

    CX_PROPERTY(Nice, Man)
    class Derived : public Base, public Base1 {
        CX_BEGIN_REFLECT()

    public:
        int derived;
    };

    CX_PROPERTY(Sick, Kicks)
    class Fields {
        CX_BEGIN_REFLECT()

    private:
        int private_simple;
        const int private_constSimple = 0;
        int* private_pointer;
        std::string private_complex;
        Base* private_complexPointer;
        const Base private_constComplex;
        int bing_bong;

    protected:
        int protected_simple;
        const int protected_constSimple = 0;
        int* protected_pointer;
        Base protected_complex;
        Base* protected_complexPointer;
        const Base protected_constComplex;

    public:
        int public_simple;
        const int public_constSimple = 0;
        int* public_pointer;
        Base public_complex;
        Base* public_complexPointer;
        const Base public_constComplex;

        CX_PROPERTY(Annotation1, Annotation2)
        int public_annotated_simple;

        CX_PROPERTY(Annotated)
        const int public_annotated_constSimple = 0;

        CX_PROPERTY(Annotation1, Annotation2, )
        int* public_annotated_pointer;

        CX_PROPERTY(Annotated)
        Base public_annotated_complex;

        CX_PROPERTY(Annotation1, Annotation2, Annotation3, Annotation4)
        Base* public_annotated_complexPointer;

        CX_PROPERTY(Annotation1)
        const Base public_annotated_constComplex;

    };

    CX_PROPERTY(Nice, Cock)
    class Methods {
        CX_BEGIN_REFLECT()

    private:
        void private_void_void() {
            std::cout << "private void()" << std::endl;
        }

    public:
        void public_void_void() {
            std::cout << "public void()" << std::endl;
        }

        void public_void_int(int a) {
            std::cout << "public void(" << a << ")" << std::endl;
        }

        int public_overload(int a) {
            std::cout << "public overload void(" << a << ")" << std::endl;
            return 3 * a;
        }

        int public_overload(int a, int b) {
            std::cout << "public overload void(" << a << ", " << b << ")" << std::endl;
            return 2 * a * b;
        }

        int public_overload(int a, int b, int c) {
            std::cout << "public overload void(" << a << ", " << b << ", " << c << ")" << std::endl;
            return a + b + c;
        }

        static void public_static_void_void() {
            std::cout << "public static void()" << std::endl;
        }

    };

    CX_PROPERTY(Nice, Guns)
    class StaticMethods {
        CX_BEGIN_REFLECT()

    private:
        static void private_void_void() {
            std::cout << "private static void()" << std::endl;
        }

        static int private_int_void() {
            std::cout << "private static int()" << std::endl;
            return 0;
        }

    public:
        static void public_void_void() {
            std::cout << "public static void()" << std::endl;
        }

        static int public_int_void() {
            std::cout << "public static int()" << std::endl;
            return 1;
        }

    };

    CX_REFLECT_ENUM()
    enum class Enum {
        ENUM1,
        ENUM2,
        ENUM3,
        ENUM4,
        ENUM5 = 0x90,
    };

}