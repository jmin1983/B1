//
// B1StringUtil.h
//
// Library: B1Base
// Package: B1Base
// Module:  B1Base
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1BASE_STRING_UTIL_H
#define _B1BASE_STRING_UTIL_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <B1Base/B1String.h>

namespace BnD {
    namespace B1StringUtil {
        B1String nextPathComponent(const B1String& path, uint32 step);  //  step == 1 means first component.
        B1String lastPathComponent(const B1String& path);
        void removeLastPathComponent(B1String* path);
        void removeLastPathSeparator(B1String* path);
        void binaryToHexaString(const std::vector<uint8>& source, B1String* result);
        void hexaStringToBinary(const B1String& source, std::vector<uint8>* result);
        bool vectorIntToString(const std::vector<int32>& source, B1String* result);
        bool stringToVectorInt(const B1String& source, std::vector<int32>* result);
    }
}   //  !BnD

#endif  // !_B1BASE_STRING_UTIL_H
