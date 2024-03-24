//
// B1TickUtil.h
//
// Library: B1Base
// Package: B1Base
// Module:  B1Base
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1BASE_TICK_UTIL_H
#define _B1BASE_TICK_UTIL_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

namespace BnD {
    namespace B1TickUtil {
        uint64 currentTick();
        uint64 diffTick(uint64 t1, uint64 t2);
    }
}   //  !BnD

#endif  // !_B1BASE_TICK_UTIL_H
