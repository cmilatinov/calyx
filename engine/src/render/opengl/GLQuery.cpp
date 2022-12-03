#include "render/opengl/GLQuery.h"
#include "render/opengl/GLUtils.h"

namespace Calyx {

    GLQuery::GLQuery() {
        glGenQueries(1, &m_queryID);
    }

    GLQuery::~GLQuery() {
        glDeleteQueries(1, &m_queryID);
    }

    void GLQuery::Begin(QueryType type) {
        m_queryType = GLQueryType(type);
        glBeginQuery(m_queryType, m_queryID);
    }

    void GLQuery::End() {
        glEndQuery(m_queryType);
    }

    uint64 GLQuery::GetValueU64() const {
        uint64 result;
        glGetQueryObjectui64v(m_queryID, GL_QUERY_RESULT, &result);
        return result;
    }

    int64 GLQuery::GetValueI64() const {
        int64 result;
        glGetQueryObjecti64v(m_queryID, GL_QUERY_RESULT, &result);
        return result;
    }

}