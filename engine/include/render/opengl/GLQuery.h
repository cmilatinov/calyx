#pragma once

#include <glad/glad.h>

#include "render/objects/Query.h"

namespace Calyx {

    class CALYX_API GLQuery : public Query {

    public:
        GLQuery();
        ~GLQuery() override;

        void Begin(QueryType type) override;
        void End() override;

        uint64 GetValueU64() const override;
        int64 GetValueI64() const override;

    private:
        GLuint m_queryID;
        GLenum m_queryType;

    };

}