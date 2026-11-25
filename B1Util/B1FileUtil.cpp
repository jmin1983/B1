//
// B1FileUtil.cpp
//
// Library: B1Util
// Package: B1Util
// Module:  B1Util
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1Util.h"
#include "B1FileUtil.h"

using namespace BnD;

B1String B1FileUtil::readFileToString(const B1String& filePath)
{
    B1String result;
    if (FILE* fp = fopen(filePath.cString(), "rb")) {
        fseek(fp, 0, SEEK_END);
        long size = ftell(fp);
        fseek(fp, 0, SEEK_SET);
        if (size < 1) {     //  empty file or ftell failed.
            fclose(fp);
            return result;
        }
        std::vector<char> buffer;
        try {
            buffer.resize(static_cast<size_t>(size) + 1, 0);    //  size_t: size can be LONG_MAX.
        }
        catch (...) {       //  invalid size(ex: directory on linux).
            fclose(fp);
            return result;
        }
        try {
            if (fread(&buffer[0], sizeof(char), size, fp) > 0) {
                result.from(&buffer[0]);
            }
        }
        catch (...) {
        }
        fclose(fp);
    }
    return result;
}

bool B1FileUtil::writeFileFromString(const B1String& string, const B1String& filePath)
{
    if (FILE* fp = fopen(filePath.cString(), "wb")) {
        fwrite(string.cString(), sizeof(char), string.length(), fp);
        fclose(fp);
        return true;
    }
    return false;
}

auto B1FileUtil::readFile(const B1String& filePath) -> std::vector<uint8>
{
    if (FILE* fp = fopen(filePath.cString(), "rb")) {
        fseek(fp, 0, SEEK_END);
        long size = ftell(fp);
        fseek(fp, 0, SEEK_SET);
        if (size < 1) {     //  empty file or ftell failed.
            fclose(fp);
            return std::vector<uint8>();
        }
        std::vector<uint8> buffer;
        try {
            buffer.resize(size);
        }
        catch (...) {       //  invalid size(ex: directory on linux).
            fclose(fp);
            return std::vector<uint8>();
        }
        if (fread(&buffer[0], sizeof(uint8), size, fp) > 0) {
            //  noop.
        }
        fclose(fp);
        return buffer;
    }
    return std::vector<uint8>();
}

bool B1FileUtil::writeFile(const std::vector<uint8>& data, const B1String& filePath)
{
    if (FILE* fp = fopen(filePath.cString(), "wb")) {
        if (data.empty() != true) {
            fwrite(&data[0], sizeof(uint8), data.size(), fp);
        }
        fclose(fp);
        return true;
    }
    return false;
}
