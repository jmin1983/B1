//
// B1Endian.h
//
// Library: B1Base
// Package: B1Base
// Module:  B1Base
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1BASE_ENDIAN_H
#define _B1BASE_ENDIAN_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#if !defined(_DO_NOT_USE_DEFAULT_ENDIAN)  //  Use little endian as default.
#define _IS_SYSTEM_BIG_ENDIAN 0
#endif

namespace BnD {
    namespace B1Endian {
        bool isSystemBigEndian();
#define DEFINE_SWAP_VALUE(type) type swap_##type(type v);
        DEFINE_SWAP_VALUE(int16);
        DEFINE_SWAP_VALUE(uint16);
        DEFINE_SWAP_VALUE(int32);
        DEFINE_SWAP_VALUE(uint32);
        DEFINE_SWAP_VALUE(int64);
        DEFINE_SWAP_VALUE(uint64);
        DEFINE_SWAP_VALUE(float32);
        DEFINE_SWAP_VALUE(float64);

#if defined(_IS_SYSTEM_BIG_ENDIAN)
#if (_IS_SYSTEM_BIG_ENDIAN)
#define TO_INT16_FOR_NETWORK(v) v
#define TO_UINT16_FOR_NETWORK(v) v
#define TO_INT32_FOR_NETWORK(v) v
#define TO_UINT32_FOR_NETWORK(v) v
#define TO_INT64_FOR_NETWORK(v) v
#define TO_UINT64_FOR_NETWORK(v) v
#define TO_FLOAT32_FOR_NETWORK(v) v
#define TO_FLOAT64_FOR_NETWORK(v) v
#else
#define TO_INT16_FOR_NETWORK(v)   B1Endian::swap_int16(v)
#define TO_UINT16_FOR_NETWORK(v)  B1Endian::swap_uint16(v)
#define TO_INT32_FOR_NETWORK(v)   B1Endian::swap_int32(v)
#define TO_UINT32_FOR_NETWORK(v)  B1Endian::swap_uint32(v)
#define TO_INT64_FOR_NETWORK(v)   B1Endian::swap_int64(v)
#define TO_UINT64_FOR_NETWORK(v)  B1Endian::swap_uint64(v)
#define TO_FLOAT32_FOR_NETWORK(v) B1Endian::swap_float32(v)
#define TO_FLOAT64_FOR_NETWORK(v) B1Endian::swap_float64(v)
#endif
#else
#define DEFINE_TO_VALUE_FOR_NETWORK(type) type to_##type##_for_network(type v);
        DEFINE_TO_VALUE_FOR_NETWORK(int16);
        DEFINE_TO_VALUE_FOR_NETWORK(uint16);
        DEFINE_TO_VALUE_FOR_NETWORK(int32);
        DEFINE_TO_VALUE_FOR_NETWORK(uint32);
        DEFINE_TO_VALUE_FOR_NETWORK(int64);
        DEFINE_TO_VALUE_FOR_NETWORK(uint64);
        DEFINE_TO_VALUE_FOR_NETWORK(float32);
        DEFINE_TO_VALUE_FOR_NETWORK(float64);
        extern bool g_isSystemBigEndian;
#define TO_INT16_FOR_NETWORK(v) B1Endian::to_int16_for_network(v)
#define TO_UINT16_FOR_NETWORK(v) B1Endian::to_uint16_for_network(v)
#define TO_INT32_FOR_NETWORK(v) B1Endian::to_int32_for_network(v)
#define TO_UINT32_FOR_NETWORK(v) B1Endian::to_uint32_for_network(v)
#define TO_INT64_FOR_NETWORK(v) B1Endian::to_int64_for_network(v)
#define TO_UINT64_FOR_NETWORK(v) B1Endian::to_uint64_for_network(v)
#define TO_FLOAT32_FOR_NETWORK(v) B1Endian::to_float32_for_network(v)
#define TO_FLOAT64_FOR_NETWORK(v) B1Endian::to_float64_for_network(v)
#endif
    }
}   //  !BnD

#endif  // !_B1BASE_ENDIAN_H
