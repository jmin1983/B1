//
// B1PerformanceProfiler.cpp
//
// Library: B1Util
// Package: B1Util
// Module:  B1Util
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1Util.h"
#include "B1PerformanceProfiler.h"

#if defined(_WIN32)
#include <Windows.h>
#pragma comment (lib, "Pdh.lib")
#else
#include <sys/types.h>
#include <sys/sysinfo.h>
#include <stdio.h>
#endif

using namespace BnD;

B1PerformanceProfiler::B1PerformanceProfiler()
    : _cpuUsage(0)
    , _memUsage(0)
    , _memTotal(0)
#if defined(_WIN32)
    , _cpuQuery(NULL)
    , _cpuTotal(NULL)
#else
    , _cpuLastTotalUser(0)
    , _cpuLastTotalUserLow(0)
    , _cpuLastTotalSys(0)
    , _cpuLastTotalIdle(0)
#endif
{
}

B1PerformanceProfiler::~B1PerformanceProfiler()
{
}

float64 B1PerformanceProfiler::getUsageCPU()
{
#if defined(_WIN32)
    if (::PdhCollectQueryData(_cpuQuery) != ERROR_SUCCESS) {
        return -1;
    }
    PDH_FMT_COUNTERVALUE counterVal;
    if (::PdhGetFormattedCounterValue(_cpuTotal, PDH_FMT_DOUBLE, NULL, &counterVal) != ERROR_SUCCESS) {
        return -1;
    }
    return counterVal.doubleValue;
#else
    uint64 cpuTotalUser(0), cpuTotalUserLow(0), cpuTotalSys(0), cpuTotalIdle(0);
    {
        FILE* file = fopen("/proc/stat", "r");
        if (NULL == file) {
            return -1;
        }
        if (0 == fscanf(file, "cpu %llu %llu %llu %llu", &cpuTotalUser, &cpuTotalUserLow, &cpuTotalSys, &cpuTotalIdle)) {
            fclose(file);
            return -1;
        }
        fclose(file);
    }
    float64 result = 0;
    if (cpuTotalUser < _cpuLastTotalUser || cpuTotalUserLow < _cpuLastTotalUserLow || cpuTotalSys < _cpuLastTotalSys || cpuTotalIdle < _cpuLastTotalIdle) {
        result = _cpuUsage; //  Overflow detection. Just skip this value.
    }
    else {
        float64 total = (cpuTotalUser - _cpuLastTotalUser) + (cpuTotalUserLow - _cpuLastTotalUserLow) + (cpuTotalSys - _cpuLastTotalSys);
        float64 percent = total;
        total += (cpuTotalIdle - _cpuLastTotalIdle);
        result = percent / total * 100;
    }
    _cpuLastTotalUser = cpuTotalUser;
    _cpuLastTotalUserLow = cpuTotalUserLow;
    _cpuLastTotalSys = cpuTotalSys;
    _cpuLastTotalIdle = cpuTotalIdle;
    return result;
#endif
}

int64 B1PerformanceProfiler::getUsageMemory()
{
#if defined(_WIN32)
    MEMORYSTATUSEX memInfo;
    memInfo.dwLength = sizeof(MEMORYSTATUSEX);
    if (::GlobalMemoryStatusEx(&memInfo) != TRUE) {
        return -1;
    }
    return (memInfo.ullTotalPhys - memInfo.ullAvailPhys) / 1024;
#else
    struct sysinfo memInfo;
    sysinfo(&memInfo);
    int64 physMemUsed = memInfo.totalram - memInfo.freeram;
    return (physMemUsed * memInfo.mem_unit) / 1024; //  Multiply in next statement to avoid int overflow on right hand side.
#endif
}

bool B1PerformanceProfiler::initialize()
{
#if defined(_WIN32)
    {
        MEMORYSTATUSEX memInfo;
        memInfo.dwLength = sizeof(MEMORYSTATUSEX);
        if (GlobalMemoryStatusEx(&memInfo) != TRUE) {
            return false;
        }
        _memTotal = memInfo.ullTotalPhys / 1024;
    }
    {
        ::PdhOpenQueryA(NULL, NULL, &_cpuQuery);
        ::PdhAddEnglishCounterA(_cpuQuery, "\\Processor(_Total)\\% Processor Time", NULL, &_cpuTotal);
        ::PdhCollectQueryData(_cpuQuery);
    }
#else
    {
        FILE* file = fopen("/proc/stat", "r");
        if (NULL == file) {
            return false;
        }
        if (0 == fscanf(file, "cpu %llu %llu %llu %llu", &_cpuLastTotalUser, &_cpuLastTotalUserLow, &_cpuLastTotalSys, &_cpuLastTotalIdle)) {
            printf("get initial cpu status failed");
        }
        fclose(file);
    }
    {
        struct sysinfo memInfo;
        sysinfo(&memInfo);
        _memTotal = memInfo.totalram;
        _memTotal = (_memTotal * memInfo.mem_unit) / 1024;  //  Multiply in next statement to avoid int overflow on right hand side.
    }
#endif
    return true;
}

void B1PerformanceProfiler::finalize()
{
#if defined(_WIN32)
    if (_cpuQuery)
        ::PdhCloseQuery(_cpuQuery);
#endif
}

void B1PerformanceProfiler::process()
{
    _cpuUsage = getUsageCPU();
    _memUsage = getUsageMemory();
}
