//
// B1Encryptor.h
//
// Library: B1Crypto
// Package: B1Crypto
// Module:  B1Crypto
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1CRYPTO_ENCRYPTOR_H
#define _B1CRYPTO_ENCRYPTOR_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

namespace BnD {
    class B1Encryptor {
    public:
        B1Encryptor();
        virtual ~B1Encryptor();
    protected:
        enum CONSTS {
            CONSTS_GAP_SIZE = 16,
            CONSTS_MD_SIZE = 64,
        };
    protected:
        const static B1String _iv;
        const static int8 _bad;
        const static std::vector<int8> _base64val;
    protected:
        static int8 decode64(uint8 c);
        static bool encryptAES256(const B1String& key, const B1String& source, std::vector<uint8>* result); //  key must be 32byte.
        static bool decryptAES256(const B1String& key, const std::vector<uint8>& source, B1String* result); //  key must be 32byte.
    public:
        static void encodeBase64(const std::vector<uint8>& source, B1String* result);
        static bool decodeBase64(const B1String& source, std::vector<uint8>* result);
        static bool encryptAES256Base64(const B1String& key, const B1String& source, B1String* result);     //  key must be 32byte.
        static bool encryptAES256Hex(const B1String& key, const B1String& source, B1String* result);        //  key must be 32byte.
        static bool decryptAES256Base64(const B1String& key, const B1String& source, B1String* result);     //  key must be 32byte.
        static bool decryptAES256Hex(const B1String& key, const B1String& source, B1String* result);        //  key must be 32byte.
        static bool encryptSha256(const B1String& source, B1String* result);
    };
}   //  !BnD

#endif  // !_B1CRYPTO_ENCRYPTOR_H
