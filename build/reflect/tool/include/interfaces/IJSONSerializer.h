#pragma once

namespace Calyx::Reflect::Tooling {

    class IJSONSerializer {

    public:
        virtual json Serialize() = 0;

    };

}
