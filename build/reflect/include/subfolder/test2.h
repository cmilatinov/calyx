#pragma once

#include "reflect.h"

namespace Calyx {

    CX_PROPERTY(Cool, Beans)
    class Test {
    CX_BEGIN_REFLECT()

    public:
        int a;
        int b;
        char c;

        CX_PROPERTY(Nice, Cock)
        void test() {}
    };

    CX_REFLECT_ENUM()
    enum class AnotherEnum {
        A = 0,
        B,
        C,
        D,
        Z = 25
    };

}