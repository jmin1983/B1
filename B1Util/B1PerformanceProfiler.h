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

#include <map>
#include <set>

namespace BnD {
    class B1PerformanceProfiler {
    public:
        B1PerformanceProfiler();
        virtual ~B1PerformanceProfiler();
    protected:
        struct CPU {
            CPU() : _usage(0), _temperature(0) {}
            float64 _usage;
            float64 _temperature;
        };
        struct MEMORY {
            MEMORY() : _usage(0), _currentProcessUsage(0), _total(0) {}
            int64 _usage;               // KB.
            int64 _currentProcessUsage; // KB.
            int64 _total;               // KB.
        };
        struct PHYSICAL_MEMORY : MEMORY {};
        struct VIRTUAL_MEMORY : MEMORY {};
    protected:
        CPU _cpu;
        int64 _memAvailable;    // KB.
        PHYSICAL_MEMORY _mem;
        VIRTUAL_MEMORY _vmem;
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
        int64 readCurrentSystemStatus(const char* filePath, const char* keyword) const;  //  return status value.
        int64 parseMemoryLineForLinux(char* line) const;    //  return memory_usage.
    protected:
        float64 getUsageCPU();
        float64 getTemperatureCPU();
        bool getUsageMemory(int64* memAvailable, int64* memoryUsage, int64* vmemoryUsage);
        bool getUsageMemoryCurrentProcess(int64* memoryUsage, int64* vmemoryUsage);
    public:
        bool initialize();
        void finalize();
        void process();
        std::pair<int64, float64> getDiskUsage(const B1String& path) const; //  return pair<capacity, used_percent>

        float64 cpuUsage() const { return _cpu._usage; }
        float64 cpuTemperature() const { return _cpu._temperature; }
        int64 memAvailable() const { return _memAvailable; }
        int64 memUsage() const { return _mem._usage; }
        int64 memCurrentProcessUsage() const { return _mem._currentProcessUsage; }
        int64 memTotal() const { return _mem._total; }
        int64 vmemUsage() const { return _vmem._usage; }
        int64 vmemCurrentProcessUsage() const { return _vmem._currentProcessUsage; }
        int64 vmemTotal() const { return _vmem._total; }
    };
}   //  !BnD

#endif  // !_B1UTIL_PERFORMANCE_PROFILER_H
