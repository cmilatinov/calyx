#pragma once

#include "utils/metrics/Types.h"
#include "utils/metrics/MetricCollector.h"

namespace Calyx::Utils {

    class CALYX_API Metrics {

    public:
        static void CollectMemoryMetrics(MemoryMetrics* metrics);
        static void CollectCPUMetrics(CPUMetrics* metrics);

    };

}