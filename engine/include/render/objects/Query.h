#pragma once

namespace Calyx {

    enum class QueryType {
        PRIMITIVES_GENERATED,
        TIME_ELAPSED
    };

    class CALYX_API Query {

    public:
        virtual ~Query() = default;

        virtual void Begin(QueryType type) = 0;
        virtual void End() = 0;

        virtual uint64 GetValueU64() const = 0;
        virtual int64 GetValueI64() const = 0;

        static Ref<Query> Create();

    };

}