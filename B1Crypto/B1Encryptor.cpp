//
// B1Encryptor.cpp
//
// Library: B1Crypto
// Package: B1Crypto
// Module:  B1Crypto
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1Crypto.h"
#include "B1Encryptor.h"

#include <B1Base/B1StringUtil.h>

#include <ctype.h>
#include <openssl/evp.h>

using namespace BnD;

const B1String B1Encryptor::_iv(CONSTS_GAP_SIZE, 'V');
const int8 B1Encryptor::_bad = -1;
const std::vector<int8> B1Encryptor::_base64val = {
    _bad, _bad, _bad, _bad, _bad, _bad, _bad, _bad, _bad, _bad, _bad, _bad, _bad, _bad, _bad, _bad,
    _bad, _bad, _bad, _bad, _bad, _bad, _bad, _bad, _bad, _bad, _bad, _bad, _bad, _bad, _bad, _bad,
    _bad, _bad, _bad, _bad, _bad, _bad, _bad, _bad, _bad, _bad, _bad,   62, _bad, _bad, _bad,   63,
      52,   53,   54,   55,   56,   57,   58,   59,   60,   61, _bad, _bad, _bad, _bad, _bad, _bad,
    _bad,    0,    1,    2,    3,    4,    5,    6,    7,    8,    9,   10,   11,   12,   13,   14,
      15,   16,   17,   18,   19,   20,   21,   22,   23,   24,   25, _bad, _bad, _bad, _bad, _bad,
    _bad,   26,   27,   28,   29,   30,   31,   32,   33,   34,   35,   36,   37,   38,   39,   40,
      41,   42,   43,   44,   45,   46,   47,   48,   49,   50,   51, _bad, _bad, _bad, _bad, _bad
};

namespace BnD {
    struct OPENSSL_CTX {
        OPENSSL_CTX() { _ctx = EVP_CIPHER_CTX_new(); EVP_CIPHER_CTX_init(_ctx); }
        ~OPENSSL_CTX() { EVP_CIPHER_CTX_cleanup(_ctx); EVP_CIPHER_CTX_free(_ctx); }
        EVP_CIPHER_CTX* _ctx;
    };
}

B1Encryptor::B1Encryptor()
{
}

B1Encryptor::~B1Encryptor()
{
}

int8 B1Encryptor::decode64(uint8 c)
{
    return isascii(c) ? _base64val[c] : _bad;
}

bool B1Encryptor::encryptAES256(const B1String& key, const B1String& source, std::vector<uint8>* result)
{
    uint32 sourceLength = source.length();
    uint32 resultLength = sourceLength;
    if (resultLength == 0 || resultLength % CONSTS_GAP_SIZE != 0) {
        resultLength += (CONSTS_GAP_SIZE - resultLength % CONSTS_GAP_SIZE);
    }
    OPENSSL_CTX ctx;

    if (EVP_EncryptInit_ex2(ctx._ctx, EVP_aes_256_cbc(), (const unsigned char*)key.cString(), (const unsigned char*)_iv.cString(), NULL) != 1) {
        return false;
    }
    {
        bool usePadding = sourceLength == 0 || sourceLength % 16 != 0;
        EVP_CIPHER_CTX_set_padding(ctx._ctx, usePadding ? 1 : 0);
    }
    int32 lengthUpdate;
    std::vector<uint8> encrypted(resultLength, 0);
    if (EVP_EncryptUpdate(ctx._ctx, &encrypted[0], &lengthUpdate, (const unsigned char*)source.cString(), sourceLength) != 1) {
        return false;
    }
    int32 lengthFinal;
    if (EVP_EncryptFinal_ex(ctx._ctx, &encrypted[0] + lengthUpdate, &lengthFinal) != 1) {
        return false;
    }
    int32 lengthTotal = lengthUpdate + lengthFinal;
    if (encrypted.size() < lengthTotal) {
        assert(false);
        return false;
    }

    result->insert(result->end(), encrypted.begin(), encrypted.begin() + lengthTotal);
    return true;
}

bool B1Encryptor::decryptAES256(const B1String& key, const std::vector<uint8>& source, B1String* result)
{
    OPENSSL_CTX ctx;
    if (EVP_DecryptInit_ex2(ctx._ctx, EVP_aes_256_cbc(), (const unsigned char*)key.cString(), (const unsigned char*)_iv.cString(), NULL) != 1) {
        return false;
    }
    EVP_CIPHER_CTX_set_padding(ctx._ctx, 0);
    int32 lengthUpdate;
    std::vector<unsigned char> decrypted(source.size() + 1, 0);
    if (EVP_DecryptUpdate(ctx._ctx, &decrypted[0], &lengthUpdate, &source[0], source.size()) != 1) {
        return false;
    }
    int32 lengthFinal;
    if (EVP_DecryptFinal_ex(ctx._ctx, &decrypted[0] + lengthUpdate, &lengthFinal) != 1) {
        return false;
    }
    int32 lengthTotal = lengthUpdate + lengthFinal;
    if (decrypted.size() < lengthTotal) {
        assert(false);
        return false;
    }
    const int32 pkcs = decrypted[lengthTotal - 1];    // PKCS#5
    int32 count = pkcs;
    for (int32 i = lengthTotal - 1; i % 16 != 0; --i) {
        if (pkcs == decrypted[i]) {
            --count;
        }
    }
    if (0 == count) {
        lengthTotal -= pkcs;
    }
    if (16 == lengthTotal) {    //  check if result string was empty string. 16 filled with '16' if result string was empty string.
        bool allPkcs = true;
        for (int32 i = 0; i < 16; ++i) {
            if (decrypted[i] != 16) {
                allPkcs = false;
                break;
            }
        }
        if (allPkcs) {
            result->clear();
            return true;
        }
    }
    decrypted[lengthTotal] = 0;
    result->from((const char*)&decrypted[0]);
    return true;
}

void B1Encryptor::encodeBase64(const std::vector<uint8>& source, B1String* result)
{
    const char base64digits[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

    auto src = &source[0];
    int32 sourceLength = source.size();
    std::vector<char> dest;
    dest.reserve(sourceLength * 2);
    for (; 3 <= sourceLength; sourceLength -= 3) {
        dest.push_back(base64digits[src[0] >> 2]);
        dest.push_back(base64digits[((src[0] << 4) & 0x30) | (src[1] >> 4)]);
        dest.push_back(base64digits[((src[1] << 2) & 0x3c) | (src[2] >> 6)]);
        dest.push_back(base64digits[src[2] & 0x3f]);
        src += 3;
    }
    if (0 < sourceLength) {
        uint8 fragment;
        dest.push_back(base64digits[src[0] >> 2]);
        fragment = (src[0] << 4) & 0x30;
        if (1 < sourceLength) {
            fragment |= src[1] >> 4;
        }
        dest.push_back(base64digits[fragment]);
        dest.push_back((sourceLength < 2) ? '=' : base64digits[(src[1] << 2) & 0x3c]);
        dest.push_back('=');
    }
    dest.push_back(0);
    try {
        result->from(&dest[0]);
    }
    catch (...) {
        result->clear();
    }
}

bool B1Encryptor::decodeBase64(const B1String& source, std::vector<uint8>* result)
{
    if (source.isEmpty()) {
        return false;
    }
    int32 sourceOffset = 0;
    std::vector<uint8> dest;
    dest.reserve(source.length());
    uint8 digit1, digit2, digit3, digit4;

    if (source.length() > 1) {
        if (source.cString()[sourceOffset] == '+' && source.cString()[sourceOffset + 1] == ' ') {
            sourceOffset += 2;
        }
    }
    if (source.cString()[sourceOffset] == '\r') {
        return true;
    }

    do {
        if (source.length() <= sourceOffset + 3) {
            return false;
        }
        digit1 = source.cString()[sourceOffset];
        if (decode64(digit1) == _bad) {
            return false;
        }
        digit2 = source.cString()[sourceOffset + 1];
        if (decode64(digit2) == _bad) {
            return false;
        }
        digit3 = source.cString()[sourceOffset + 2];
        if (digit3 != '=' && decode64(digit3) == _bad) {
            return false;
        }
        digit4 = source.cString()[sourceOffset + 3];
        if (digit4 != '=' && decode64(digit4) == _bad) {
            return false;
        }
        sourceOffset += 4;
        dest.push_back((decode64(digit1) << 2) | (decode64(digit2) >> 4));
        if (digit3 != '=') {
            dest.push_back(((decode64(digit2) << 4) & 0xf0) | (decode64(digit3) >> 2));
            if (digit4 != '=') {
                dest.push_back(((decode64(digit3) << 6) & 0xc0) | decode64(digit4));
            }
        }
    } while (source.length() > sourceOffset && source.cString()[sourceOffset] != '\r' && digit4 != '=');

    result->swap(dest);
    return true;
}

bool B1Encryptor::encryptAES256Base64(const B1String& key, const B1String& source, B1String* result)
{
    std::vector<uint8> encrypted;
    if (encryptAES256(key, source, &encrypted) != true) {
        return false;
    }
    encodeBase64(encrypted, result);
    return true;
}

bool B1Encryptor::encryptAES256Hex(const B1String& key, const B1String& source, B1String* result)
{
    std::vector<uint8> encrypted;
    if (encryptAES256(key, source, &encrypted) != true) {
        return false;
    }
    B1StringUtil::binaryToHexaString(encrypted, result);
    return true;
}

bool B1Encryptor::decryptAES256Base64(const B1String& key, const B1String& source, B1String* result)
{
    std::vector<uint8> decoded;
    if (decodeBase64(source, &decoded) != true) {
        return false;
    }
    return decryptAES256(key, decoded, result);
}

bool B1Encryptor::decryptAES256Hex(const B1String& key, const B1String& source, B1String* result)
{
    std::vector<unsigned char> encrypted;
    B1StringUtil::hexaStringToBinary(source, &encrypted);
    return decryptAES256(key, encrypted, result);
}

bool B1Encryptor::encryptSha256(const B1String& source, B1String* result)
{
    const auto md = EVP_get_digestbyname("sha256");
    if (NULL == md) {
        return false;
    }
    auto mdCTX = EVP_MD_CTX_new();
    EVP_DigestInit_ex(mdCTX, md, NULL);
    EVP_DigestUpdate(mdCTX, source.cString(), source.length());

    uint8 hash[CONSTS_MD_SIZE] = {0};
    uint32 hashLen = 0;
    EVP_DigestFinal_ex(mdCTX, hash, &hashLen);
    EVP_MD_CTX_free(mdCTX);
    if (hashLen >= CONSTS_MD_SIZE) {
        return false;
    }
    std::vector<uint8> encrypted(hash, hash + hashLen);
    B1StringUtil::binaryToHexaString(encrypted, result);
    return true;
}
