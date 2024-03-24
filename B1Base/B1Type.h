//
// B1Type.h
//
// Library: B1Base
// Package: B1Base
// Module:  B1Base
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1TYPE_H
#define _B1TYPE_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <climits>
#include <stdint.h>

#if defined(_WIN32)
#include <ctime>
#endif

namespace BnD {
    typedef   signed char       int8;
    typedef unsigned char      uint8;
    typedef   signed short      int16;
    typedef unsigned short     uint16;
    typedef   signed int        int32;
    typedef unsigned int       uint32;
    typedef          float    float32;
    typedef          double   float64;
#if defined(_WIN32)
    typedef   signed __int64    int64;
    typedef unsigned __int64   uint64;
#else
    typedef   signed long long  int64;
    typedef unsigned long long uint64;
#endif

#if defined(_WIN32)
    //typedef      __time32_t  time_t32;    //  deprecated
    typedef      __time64_t  time_t64;
#else
    //typedef          int32   time_t32;    //  deprecated
    typedef          int64   time_t64;
#endif

#if defined(_WIN32)
#define CONST_INT64(i)      i##i64
#define CONST_UINT64(i)     i##ui64
#else
#define CONST_INT64(i)      i##LL
#define CONST_UINT64(i)     i##ULL
#endif

#ifndef INT8_MIN
    const  int8  INT8_MIN = SCHAR_MIN;
#endif
#ifndef INT8_MAX
    const  int8  INT8_MAX = SCHAR_MAX;
#endif
#ifndef UINT8_MAX
    const uint8  UINT8_MAX = UCHAR_MAX;
#endif
#ifndef INT16_MIN
    const  int16 INT16_MIN = SHRT_MIN;
#endif
#ifndef INT16_MAX
    const  int16 INT16_MAX = SHRT_MAX;
#endif
#ifndef UINT16_MAX
    const uint16 UINT16_MAX = USHRT_MAX;
#endif
#ifndef INT32_MIN
    const  int32 INT32_MIN = INT_MIN;
#endif
#ifndef INT32_MAX
    const  int32 INT32_MAX = INT_MAX;
#endif
#ifndef UINT32_MAX
    const uint32 UINT32_MAX = UINT_MAX;
#endif

#if defined(_WIN32)
#ifndef INT64_MIN
    const  int64 INT64_MIN = _I64_MIN;
#endif
#ifndef INT64_MAX
    const  int64 INT64_MAX = _I64_MAX;
#endif
#ifndef UINT64_MAX
    const uint64 UINT64_MAX = _UI64_MAX;
#endif
#elif defined(__linux__) && defined(_USE_ISOC99)
    const  int64 INT64_MIN = LLONG_MIN;
    const  int64 INT64_MAX = LLONG_MAX;
    const uint64 UINT64_MAX = ULLONG_MAX;
#else
#ifndef INT64_MIN
    const  int64 INT64_MIN = -9223372036854775807LL - 1;
#endif
#ifndef INT64_MAX
    const  int64 INT64_MAX = 9223372036854775807LL;
#endif
#ifndef UINT64_MAX
    const uint64 UINT64_MAX = 0xffffffffffffffffLLU;
#endif
#endif

#if defined(_WIN32)
#ifndef NOMINMAX
#define NOMINMAX
#endif
#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif
#endif

#if defined(__linux__)
#if !defined(NULL)
#define NULL 0
#endif
#endif

}   //  !BnD

#endif  // !_B1TYPE_H
