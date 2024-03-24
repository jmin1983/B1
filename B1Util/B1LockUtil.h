//
// B1LockUtil.h
//
// Library: B1Util
// Package: B1Util
// Module:  B1Util
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1UTIL_LOCK_UTIL_H
#define _B1UTIL_LOCK_UTIL_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <B1Base/B1Lock.h>

namespace BnD {
    template<typename T>
    struct B1LockUtil {
        B1LockUtil() {}
        B1LockUtil(const T& data) : _data(data), _dataLock() {}
        T _data;
        mutable B1Lock _dataLock;
        bool isSame(const T& data) const
        {
            B1AutoLock al(_dataLock);
            return _data == data;
        }
        const T& data() const
        {
            B1AutoLock al(_dataLock);
            return _data;
        }
    };
}   //  !BnD

#endif  // !_B1UTIL_LOCK_UTIL_H
