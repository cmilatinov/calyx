#pragma once

namespace reflect::tooling {

    template<typename T>
    class IDeclProcessor {

    public:
        virtual void Accept(T* decl) = 0;

    };

}
