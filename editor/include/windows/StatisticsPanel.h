#pragma once

#include <imgui.h>

#include "render/objects/Query.h"
#include "utils/metrics/Metrics.h"
#include "utils/metrics/MetricCollector.h"
#include "utils/Format.h"
#include "utils/Graph.h"

namespace Calyx::Editor {

    class StatisticsPanel {

    public:
        StatisticsPanel();

        void Draw();

        void BeginFrame();
        void EndFrame();

    private:
        MetricCollector<uint64> m_memCollector;
        MetricCollector<double> m_cpuCollector;

        // Metrics
        MemoryMetrics m_memory{};
        CPUMetrics m_cpu{};

        // Queries
        Ref<Query> m_timeElapsedQuery;
        Ref<Query> m_primitivesQuery;

        // Values
        Chrono::time_point<Clock> m_lastCPUTime;
        uint64 m_gpuTime = 0;
        uint64 m_cpuTime = 0;
        uint64 m_primitives = 0;

    };

}