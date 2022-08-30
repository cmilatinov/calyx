#pragma once

namespace reflect::tooling {

    class IJSONSerializer {

    public:
        virtual json Serialize() = 0;

    };

}
