//
// B1Compressor.h
//
// Library: B1Util
// Package: B1Util
// Module:  B1Util
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1UTIL_COMPRESSOR_H
#define _B1UTIL_COMPRESSOR_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

namespace BnD {
    namespace B1Compressor {
        std::vector<uint8> compress(const std::vector<uint8>& data, const size_t bufferSize = 1024 * 256);      //  return empty if failed.
        std::vector<uint8> decompress(const std::vector<uint8>& data, const size_t bufferSize = 1024 * 256);    //  return empty if failed.
        bool compressToFile(const std::vector<uint8>& data, const B1String& filePath);
        std::vector<uint8> decompressFromFile(const B1String& filePath);
        bool compressFromFileToFile(const B1String& filePathFrom, const B1String& filePathTo);
        bool decompressFromFileToFile(const B1String& filePathFrom, const B1String& filePathTo);
    };
}   //  !BnD

#endif  // !_B1UTIL_COMPRESSOR_H
