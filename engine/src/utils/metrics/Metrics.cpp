#include "utils/metrics/Metrics.h"

#ifdef CX_PLATFORM_WINDOWS
#include "psapi.h"
#endif

namespace Calyx {

    void Metrics::CollectMemoryMetrics(MemoryMetrics* metrics) {
#ifdef CX_PLATFORM_WINDOWS
        MEMORYSTATUSEX memInfo;
        memInfo.dwLength = sizeof(MEMORYSTATUSEX);
        GlobalMemoryStatusEx(&memInfo);

        PROCESS_MEMORY_COUNTERS_EX pmc;
        GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc));

        metrics->Total = memInfo.ullTotalPhys;
        metrics->Used = memInfo.ullTotalPhys - memInfo.ullAvailPhys;
        metrics->ProcessUsed = pmc.WorkingSetSize;
#endif
    }

    void Metrics::CollectCPUMetrics(CPUMetrics* metrics) {
#ifdef CX_PLATFORM_WINDOWS
        static HANDLE s_currentProc = INVALID_HANDLE_VALUE;
        static int s_numProcessors = 0;
        static struct {
            ULARGE_INTEGER LastCPU;
            ULARGE_INTEGER LastSysCPU;
            ULARGE_INTEGER LastUserCPU;
        } s_procTimes;

        if (s_currentProc == INVALID_HANDLE_VALUE) {
            SYSTEM_INFO sysInfo;
            GetSystemInfo(&sysInfo);
            s_currentProc = GetCurrentProcess();
            s_numProcessors = sysInfo.dwNumberOfProcessors;
        }

        FILETIME ftime, fsys, fuser;
        ULARGE_INTEGER now, sys, user;
        GetSystemTimeAsFileTime(&ftime);
        memcpy(&now, &ftime, sizeof(FILETIME));
        GetProcessTimes(s_currentProc, &ftime, &ftime, &fsys, &fuser);
        memcpy(&sys, &fsys, sizeof(FILETIME));
        memcpy(&user, &fuser, sizeof(FILETIME));

        metrics->ProcessUsage =
            (sys.QuadPart - s_procTimes.LastSysCPU.QuadPart) +
            (user.QuadPart - s_procTimes.LastUserCPU.QuadPart);
        metrics->ProcessUsage /= (now.QuadPart - s_procTimes.LastCPU.QuadPart);
        metrics->ProcessUsage /= s_numProcessors;

        s_procTimes.LastCPU = now;
        s_procTimes.LastSysCPU = sys;
        s_procTimes.LastUserCPU = user;
#endif
    }

}