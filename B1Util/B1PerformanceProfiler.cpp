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
#include <Psapi.h>
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
    , _memCurrentProcessUsage(0)
    , _memTotal(0)
    , _vmemUsage(0)
    , _vmemCurrentProcessUsage(0)
    , _vmemTotal(0)
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

int64 B1PerformanceProfiler::readCurrentProcessStatus(const char* keyword) const
{
    FILE* file = fopen("/proc/self/status", "r");
    if (NULL == file) {
        return 0;
    }
    int64 result = 0;
    char line[128] = {0};
    while (fgets(line, 128, file) != NULL) {
        if (strncmp(line, keyword, strlen(keyword)) == 0) {
            result = parseMemoryLineForLinux(line);
            break;
        }
    }
    fclose(file);
    return result;
}

int64 B1PerformanceProfiler::parseMemoryLineForLinux(char* line) const
{
    // This assumes that a digit will be found and the line ends in " Kb".
    auto i = strlen(line);
    const char* p = line;
    while (*p < '0' || *p > '9') {
        p++;
    }
    line[i - 3] = '\0';
    return atoll(p);
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

bool B1PerformanceProfiler::getUsageMemory(int64* memoryUsage, int64* vmemoryUsage)
{
#if defined(_WIN32)
    MEMORYSTATUSEX memInfo;
    memInfo.dwLength = sizeof(MEMORYSTATUSEX);
    if (::GlobalMemoryStatusEx(&memInfo) != TRUE) {
        return false;
    }
    *memoryUsage = (memInfo.ullTotalPhys - memInfo.ullAvailPhys) / 1024;
    *vmemoryUsage = (memInfo.ullTotalPageFile - memInfo.ullAvailPageFile) / 1024;
#else
    struct sysinfo memInfo;
    sysinfo(&memInfo);
    int64 physMemUsed = memInfo.totalram - memInfo.freeram;
    *memoryUsage = (physMemUsed * memInfo.mem_unit) / 1024; //  Multiply in next statement to avoid int overflow on right hand side.
    int64 virtualMemUsed = memInfo.totalram - memInfo.freeram;
    virtualMemUsed += memInfo.totalswap - memInfo.freeswap;
    *vmemoryUsage = (virtualMemUsed *= memInfo.mem_unit) / 1024;
#endif
    return true;
}

bool B1PerformanceProfiler::getUsageMemoryCurrentProcess(int64* memoryUsage, int64* vmemoryUsage)
{
#if defined(_WIN32)
    PROCESS_MEMORY_COUNTERS_EX pmc;
    if (::GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc)) != TRUE) {
        return false;
    }
    *memoryUsage = pmc.WorkingSetSize / 1024;
    *vmemoryUsage = pmc.PrivateUsage / 1024;
#else
    *memoryUsage = readCurrentProcessStatus("VmRSS:");
    *vmemoryUsage = readCurrentProcessStatus("VmSize:");
#endif
    return true;
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
        _vmemTotal = memInfo.ullTotalPageFile / 1024;
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
        _memTotal = memInfo.totalram;   //  to avoid overflow.
        _memTotal = (_memTotal * memInfo.mem_unit) / 1024;  //  Multiply in next statement to avoid int overflow on right hand side.
        _vmemTotal = memInfo.totalram;  //  to avoid overflow.
        _vmemTotal += memInfo.totalswap;
        _vmemTotal = (_vmemTotal * memInfo.mem_unit) / 1024;
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
    {
        int64 memoryUsage = 0, vmemoryUsage = 0;
        if (getUsageMemory(&memoryUsage, &vmemoryUsage)) {
            _memUsage = memoryUsage;
            _vmemUsage = vmemoryUsage;
        }
        else {
            _memUsage = 0;
            _vmemUsage = 0;
        }
    }
    {
        int64 memoryUsage = 0, vmemoryUsage = 0;
        if (getUsageMemoryCurrentProcess(&memoryUsage, &vmemoryUsage)) {
            _memCurrentProcessUsage = memoryUsage;
            _vmemCurrentProcessUsage = vmemoryUsage;
        }
        else {
            _memCurrentProcessUsage = 0;
            _vmemCurrentProcessUsage = 0;
        }
    }
}
