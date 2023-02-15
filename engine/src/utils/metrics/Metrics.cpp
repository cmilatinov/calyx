#include "utils/metrics/Metrics.h"

#ifdef CX_PLATFORM_WINDOWS
#include "psapi.h"
#elif defined(CX_PLATFORM_LINUX)

#include "sys/types.h"
#include "sys/sysinfo.h"
#include "sys/times.h"

#endif

namespace Calyx::Utils {

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
#elif defined(CX_PLATFORM_LINUX)
        struct sysinfo memInfo{};
        sysinfo(& memInfo);
        metrics->Total = memInfo.totalram;
        metrics->Total *= memInfo.mem_unit;
        metrics->Used = memInfo.totalram - memInfo.freeram;
        metrics->Used *= memInfo.mem_unit;

        std::ifstream memInfoFile("/proc/meminfo");
        for (String line; std::getline(memInfoFile, line);) {
            if (line.starts_with("MemAvailable:")) {
                StringStream ss(line);
                ss >> line >> metrics->Used;
                metrics->Used *= 1024;
                metrics->Used = metrics->Total - metrics->Used;
                break;
            }
        }

        std::ifstream statusFile("/proc/self/status");
        for (String line; std::getline(statusFile, line);) {
            if (line.starts_with("VmRSS:")) {
                StringStream ss(line);
                ss >> line >> metrics->ProcessUsed;
                metrics->ProcessUsed *= 1024;
                break;
            }
        }
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
#elif defined(CX_PLATFORM_LINUX)
        static int s_numProcessors = 0;
        static struct {
            clock_t LastCPU;
            clock_t LastSysCPU;
            clock_t LastUserCPU;
        } s_procTimes;
        struct tms timeSample{};

        if (s_numProcessors == 0) {
            s_procTimes.LastCPU = times(&timeSample);
            s_procTimes.LastSysCPU = timeSample.tms_stime;
            s_procTimes.LastUserCPU = timeSample.tms_utime;

            std::ifstream cpuInfoFile("/proc/cpuinfo");
            for (String line; std::getline(cpuInfoFile, line);) {
                if (line.starts_with("processor")) {
                    s_numProcessors++;
                }
            }
        }

        // Overflow detection. Just skip this value.
        clock_t now = times(&timeSample);
        if (now <= s_procTimes.LastCPU ||
            timeSample.tms_stime < s_procTimes.LastSysCPU ||
            timeSample.tms_utime < s_procTimes.LastUserCPU) {
            return;
        }

        metrics->ProcessUsage = (timeSample.tms_stime - s_procTimes.LastSysCPU) +
                                (timeSample.tms_utime - s_procTimes.LastUserCPU);
        metrics->ProcessUsage /= (now - s_procTimes.LastCPU);
        metrics->ProcessUsage /= s_numProcessors;
#endif
    }

}