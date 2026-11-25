//
// B1CommonUtil.h
//
// Library: B1Util
// Package: B1Util
// Module:  B1Util
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1UTIL_COMMON_UTIL_H
#define _B1UTIL_COMMON_UTIL_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <B1Base/B1String.h>

#include <vector>

namespace BnD {
    namespace B1CommonUtil {
        int32 isInRange(float64 l, float64 r, float64 range);   //  return -1 if l > r, return 1 if l < r, return 0 if same.
        std::vector<std::vector<uint8> > splitData(std::vector<uint8>&& data, size_t limitSize);
        B1String generateUUID();   //  random UUID (RFC 4122 version 4, 128 bits), lower case "xxxxxxxx-xxxx-4xxx-yxxx-xxxxxxxxxxxx". unique without any managing system.
    };
}   //  !BnD

#endif  // !_B1UTIL_COMMON_UTIL_H
