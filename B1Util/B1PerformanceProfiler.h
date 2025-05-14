//
// B1PerformanceProfiler.h
//
// Library: B1Util
// Package: B1Util
// Module:  B1Util
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1UTIL_PERFORMANCE_PROFILER_H
#define _B1UTIL_PERFORMANCE_PROFILER_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#if defined(_WIN32)
#include <Pdh.h>
#endif

namespace BnD {
    class B1PerformanceProfiler {
    public:
        B1PerformanceProfiler();
        virtual ~B1PerformanceProfiler();
    protected:
        float64 _cpuUsage;
        int64 _memUsage;                // KB.
        int64 _memCurrentProcessUsage;  // KB.
        int64 _memTotal;                // KB.
        int64 _vmemUsage;               // KB.
        int64 _vmemCurrentProcessUsage; // KB.
        int64 _vmemTotal;               // KB.
    private:
#if defined(_WIN32)
        PDH_HQUERY _cpuQuery;
        PDH_HCOUNTER _cpuTotal;
#else
        uint64 _cpuLastTotalUser;
        uint64 _cpuLastTotalUserLow;
        uint64 _cpuLastTotalSys;
        uint64 _cpuLastTotalIdle;
#endif
    private:
        int64 readCurrentProcessStatus(const char* keyword) const;  //  return status value.
        int64 parseMemoryLineForLinux(char* line) const;    //  return memory_usage.
    protected:
        float64 getUsageCPU();
        bool getUsageMemory(int64* memoryUsage, int64* vmemoryUsage);
        bool getUsageMemoryCurrentProcess(int64* memoryUsage, int64* vmemoryUsage);
    public:
        bool initialize();
        void finalize();
        void process();

        float64 cpuUsage() const { return _cpuUsage; }
        int64 memUsage() const { return _memUsage; }
        int64 memCurrentProcessUsage() const { return _memCurrentProcessUsage; }
        int64 memTotal() const { return _memTotal; }
        int64 vmemUsage() const { return _vmemUsage; }
        int64 vmemCurrentProcessUsage() const { return _vmemCurrentProcessUsage; }
        int64 vmemTotal() const { return _vmemTotal; }
    };
}   //  !BnD

#endif  // !_B1UTIL_PERFORMANCE_PROFILER_H
