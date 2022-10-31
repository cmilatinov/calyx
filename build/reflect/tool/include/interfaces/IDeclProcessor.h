#pragma once

namespace Calyx::Reflect::Tooling {

    template<typename T>
    class IDeclProcessor {

    public:
        virtual void Accept(T* decl) = 0;

    };

}
