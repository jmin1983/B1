//
// B1CommonUtil.cpp
//
// Library: B1Util
// Package: B1Util
// Module:  B1Util
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1Util.h"
#include "B1CommonUtil.h"

namespace BnD {
    namespace B1CommonUtil {
        int32 isInRange(float64 l, float64 r, float64 range)    //  -1, 0, 1
        {
            if (l > r) {
                if ((l - r) > range)
                    return -1;
            }
            else if (r > l) {
                if ((r - l) > range)
                    return 1;
            }
            return 0;
        }

    };
}   //  !BnD