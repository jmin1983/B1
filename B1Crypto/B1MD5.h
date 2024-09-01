//
// B1MD5.h
//
// Library: B1Crypto
// Package: B1Crypto
// Module:  B1Crypto
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1CRYPTO_MD5_H
#define _B1CRYPTO_MD5_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

namespace BnD {
    namespace B1MD5 {
        B1String getFileMD5(const B1String& filePath);
    };
}   //  !BnD

#endif  // !_B1CRYPTO_MD5_H
