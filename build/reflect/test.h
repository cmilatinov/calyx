#pragma once

#define REFLECT(...)

#define CX_PROPERTY(...)

namespace Calyx {

    class Base {

    };

    REFLECT()
    class ReflectTest : public Base {
    private:

        CX_PROPERTY(a, b)
        int field;

    public:
        CX_PROPERTY(asdas, asda)
        void foo();

        static void bar();

        class Inner {
        private:
            float field;
        };
    };

}