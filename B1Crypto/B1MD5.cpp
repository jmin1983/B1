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

#include <openssl/md5.h>

using namespace BnD;

B1String B1MD5::getFileMD5(const std::vector<uint8>& data)
{
    if (data.empty()) {
        return "";
    }
    std::vector<uint8> hash(MD5_DIGEST_LENGTH);
    MD5_CTX md5;
    MD5_Init(&md5);
    MD5_Update(&md5, &data[0], data.size());
    MD5_Final(&hash[0], &md5);

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
