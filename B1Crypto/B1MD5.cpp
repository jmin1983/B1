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

#include <openssl/evp.h>

using namespace BnD;

B1String B1MD5::getFileMD5(const std::vector<uint8>& data)
{
    if (data.empty()) {
        return "";
    }
    EVP_MD_CTX* context = EVP_MD_CTX_new();
    const EVP_MD* md = EVP_md5();
    unsigned char md_value[EVP_MAX_MD_SIZE];
    unsigned int mdLen = 0;
    std::string output;

    EVP_DigestInit_ex2(context, md, NULL);
    EVP_DigestUpdate(context, &data[0], data.size());
    EVP_DigestFinal_ex(context, md_value, &mdLen);
    EVP_MD_CTX_free(context);

    output.resize(mdLen * 2);
    for (unsigned int i = 0; i < mdLen; ++i) {
        std::sprintf(&output[i * 2], "%02x", md_value[i]);
    }
    return output;
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
