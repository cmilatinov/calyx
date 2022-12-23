#pragma once

namespace Calyx::Utils {

    struct MemoryMetrics {
        uint64 Total;
        uint64 Used;
        uint64 ProcessUsed;
    };

    struct CPUMetrics {
        double ProcessUsage;
    };

}
