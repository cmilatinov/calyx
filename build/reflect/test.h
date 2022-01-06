#pragma once

#define REFLECT(...)

#define CX_PROPERTY(...)

namespace Calyx {

    class Base {

    };

    REFLECT(

            )
    class ReflectTest : public Base {
    private:

        CX_PROPERTY(a, b)
        int field;

    public:
        CX_PROPERTY(asdas, asda)
        void foo(int, float);

        static void bar();

        class Inner {
        private:
            float field;
        };
    };

}

class AnotherTest {

};

struct asdk {

    CX_PROPERTY(Cool  , Beans)
    bool (*pFunc)();

    CX_PROPERTY(    ANOTHER, ONE)
    CX_PROPERTY(Hello, World)
    static void AnActualFn() {}

    void (*hehe)();

};