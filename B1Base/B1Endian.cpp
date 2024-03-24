//
// B1Endian.cpp
//
// Library: B1Base
// Package: B1Base
// Module:  B1Base
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1Base.h"
#include "B1Endian.h"

using namespace BnD;

bool B1Endian::isSystemBigEndian()
{
    int32 i = 0x00000001;
    return (0 == *(int8*)&i);
}

int16 B1Endian::swap_int16(int16 i)
{
    return (int16)swap_uint16((uint16)i);
}

uint16 B1Endian::swap_uint16(uint16 i)
{
#if defined(__i386__) && defined(__GNUC__)
    __asm__("xchgb %b0, %h0" : "+q" (i));
    return i;
#elif defined(__ppc__) && defined(__GNUC__)
    uint16 result;
    __asm__("lhbrx %0,0,%1" : "=r" (result) : "r" (&i), "m" (i));
    return result;
#else
    return ((i >> 8) & 0x00FF) | ((i << 8) & 0xFF00);
#endif
}

int32 B1Endian::swap_int32(int32 i)
{
    return (int32)swap_uint32((uint32)i);
}

uint32 B1Endian::swap_uint32(uint32 i)
{
#if defined(__i386__) && defined(__GNUC__)
    __asm__("bswap %0" : "+r" (i));
    return i;
#elif defined(__ppc__) && defined(__GNUC__)
    uint32 result;
    __asm__("lwbrx %0,0,%1" : "=r" (result) : "r" (&i), "m" (i));
    return result;
#else
    return ((i >> 24) & 0x000000FF) | ((i << 24) & 0xFF000000) | ((i >> 8) & 0x0000FF00) | ((i << 8) & 0x00FF0000);
#endif
}

int64 B1Endian::swap_int64(int64 i)
{
    return (int64)swap_uint64((uint64)i);
}

uint64 B1Endian::swap_uint64(uint64 i)
{
    union Swap {
        uint64 sv;
        uint32 ul[2];
    } tmp, result;
    tmp.sv = i;
    result.ul[0] = swap_uint32(tmp.ul[1]);
    result.ul[1] = swap_uint32(tmp.ul[0]);
    return result.sv;
}

float32 B1Endian::swap_float32(float32 f)
{
    float32 result;
    uint8* r = reinterpret_cast<uint8*>(&result);
    uint8* p = reinterpret_cast<uint8*>(&f);
    r[0] = p[3], r[1] = p[2], r[2] = p[1], r[3] = p[0];
    return result;
}

float64 B1Endian::swap_float64(float64 f)
{
    float64 result;
    uint8* r = reinterpret_cast<uint8*>(&result);
    uint8* p = reinterpret_cast<uint8*>(&f);
    r[0] = p[7], r[1] = p[6], r[2] = p[5], r[3] = p[4];
    r[4] = p[3], r[5] = p[2], r[6] = p[1], r[7] = p[0];
    return result;
}

#if !defined(_IS_SYSTEM_BIG_ENDIAN)
#define IMPLEMENT_TO_VALUE_FOR_NETWORK(type)\
    type B1Endian::to_##type##_for_network(type v)\
    {\
        if (g_isSystemBigEndian)\
            return v;\
        else\
            return swap_##type(v); \
    }
IMPLEMENT_TO_VALUE_FOR_NETWORK(int16);
IMPLEMENT_TO_VALUE_FOR_NETWORK(uint16);
IMPLEMENT_TO_VALUE_FOR_NETWORK(int32);
IMPLEMENT_TO_VALUE_FOR_NETWORK(uint32);
IMPLEMENT_TO_VALUE_FOR_NETWORK(int64);
IMPLEMENT_TO_VALUE_FOR_NETWORK(uint64);
IMPLEMENT_TO_VALUE_FOR_NETWORK(float32);
IMPLEMENT_TO_VALUE_FOR_NETWORK(float64);
bool B1Endian::g_isSystemBigEndian = B1Endian::isSystemBigEndian();
#endif
