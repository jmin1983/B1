//
// B1TickUtil.cpp
//
// Library: B1Base
// Package: B1Base
// Module:  B1Base
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1Base.h"
#include "B1TickUtil.h"

#include <limits>
#if defined(_WIN32)
#include <Windows.h>
#else
#include <time.h>
#endif

using namespace BnD;

uint64 B1TickUtil::currentTick()
{
#if defined(_WIN32)
    return ::GetTickCount64();  //  see timeBeginPeriod() for Windows.
#else
    struct timespec tp;
    clock_gettime(CLOCK_MONOTONIC, &tp);
    return tp.tv_sec * 1000ull + tp.tv_nsec / 1000ull / 1000ull;
#endif
}

uint64 B1TickUtil::diffTick(uint64 t1, uint64 t2)
{
    return (t1 < t2) ? t2 - t1 : t1 - t2;
}
