//
// B1FileUtil.h
//
// Library: B1Util
// Package: B1Util
// Module:  B1Util
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1UTIL_FILE_UTIL_H
#define _B1UTIL_FILE_UTIL_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

namespace BnD {
    namespace B1FileUtil {
        B1String readFileToString(const B1String& filePath);
        bool writeFileFromString(const B1String& string, const B1String& filePath);
        std::vector<uint8> readFile(const B1String& filePath);
        bool writeFile(const std::vector<uint8>& data, const B1String& filePath);
        bool readFileVersion(const B1String& filePath, int32* version, B1String* buildDate);
        bool writeFileVersion(int32 version, const B1String& buildDate, const B1String& filePath);
    };
}   //  !BnD

#endif  // !_B1UTIL_FILE_UTIL_H
