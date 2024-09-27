//
// B1MD5.cpp
//
// Library: B1Crypto
// Package: B1Crypto
// Module:  B1Crypto
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1Crypto.h"
#include "B1MD5.h"

#include <B1Base/B1StringUtil.h>

#include <openssl/evp.h>

using namespace BnD;

B1String B1MD5::getFileMD5(const std::vector<uint8>& data)
{
    if (data.empty()) {
        return "";
    }
    EVP_MD_CTX* mdctx = EVP_MD_CTX_new();

    EVP_DigestInit_ex(mdctx, EVP_md5(), NULL);
    EVP_DigestUpdate(mdctx, &data[0], data.size());
    unsigned int md5DigestLength = EVP_MD_size(EVP_md5());
    unsigned char* md5Digest = (unsigned char*)OPENSSL_malloc(md5DigestLength);
    EVP_DigestFinal_ex(mdctx, md5Digest, &md5DigestLength);
    std::vector<uint8> hash(md5Digest, md5Digest + md5DigestLength);
    OPENSSL_free(md5Digest);
    EVP_MD_CTX_free(mdctx);

    B1String result;
    B1StringUtil::binaryToHexaString(hash, &result);
    return result;
}

B1String B1MD5::getFileMD5(const B1String& filePath)
{
    std::vector<uint8> buffer;
    if (FILE* fp = fopen(filePath.cString(), "rb")) {
        fseek(fp, 0, SEEK_END);
        long size = ftell(fp);
        fseek(fp, 0, SEEK_SET);
        buffer.resize(size);
        try {
            if (fread(&buffer[0], sizeof(uint8), size, fp) > 0) {
                //  noop.
            }
        }
        catch (...) {
        }
        fclose(fp);
    }
    return getFileMD5(buffer);
}
