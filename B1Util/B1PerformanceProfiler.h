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
        int64 _memUsage;    //  KB
        int64 _memTotal;    //  KB
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
    protected:
        float64 getUsageCPU();
        int64 getUsageMemory();
    public:
        bool initialize();
        void finalize();
        void process();

        float64 cpuUsage() const { return _cpuUsage; }
        int64 memUsage() const { return _memUsage; }
        int64 memTotal() const { return _memTotal; }
    };
}   //  !BnD

#endif  // !_B1UTIL_PERFORMANCE_PROFILER_H
