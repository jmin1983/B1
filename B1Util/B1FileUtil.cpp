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

#include <B1Base/B1Endian.h>

using namespace BnD;

B1String B1FileUtil::readFileToString(const B1String& filePath)
{
    B1String result;
    if (FILE* fp = fopen(filePath.cString(), "rb")) {
        fseek(fp, 0, SEEK_END);
        long size = ftell(fp);
        fseek(fp, 0, SEEK_SET);
        std::vector<char> buffer(size + 1, 0);
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
        std::vector<uint8> buffer(size);
        try {
            if (fread(&buffer[0], sizeof(uint8), size, fp) > 0) {
                //  noop.
            }
        }
        catch (...) {
        }
        fclose(fp);
        return buffer;
    }
    return std::vector<uint8>();
}

bool B1FileUtil::writeFile(const std::vector<uint8>& data, const B1String& filePath)
{
    if (FILE* fp = fopen(filePath.cString(), "wb")) {
        fwrite(&data[0], sizeof(uint8), data.size(), fp);
        fclose(fp);
        return true;
    }
    return false;
}

bool B1FileUtil::readFileVersion(const B1String& filePath, int32* version, B1String* buildDate)
{
    auto data = readFile(filePath);
    int32 versionData = 0;
    int32 buildDateLengthData = 0;
    if (data.size() < sizeof(versionData) + sizeof(buildDateLengthData)) {
        return false;
    }
    memcpy(&versionData, &data[0], sizeof(versionData));
    memcpy(&buildDateLengthData, &data[sizeof(versionData)], sizeof(buildDateLengthData));
    if (version) {
        *version = TO_INT32_FOR_NETWORK(versionData);
    }
    int32 buildDateLength = TO_INT32_FOR_NETWORK(buildDateLengthData);
    if (data.size() < sizeof(versionData) + sizeof(buildDateLengthData) + buildDateLength) {
        return false;
    }
    if (buildDate) {
        std::vector<char> temp(data.begin() + sizeof(versionData) + sizeof(buildDateLengthData), data.end());
        temp.push_back(0);
        buildDate->from(&temp[0]);
    }
    return true;
}

bool B1FileUtil::writeFileVersion(int32 version, const B1String& buildDate, const B1String& filePath)
{
    std::vector<uint8> data;
    int32 versionData = TO_INT32_FOR_NETWORK(version);
    int32 buildDateLengthData = TO_INT32_FOR_NETWORK(buildDate.length());
    data.insert(data.end(), (uint8*)&versionData, (uint8*)&versionData + sizeof(versionData));
    data.insert(data.end(), (uint8*)&buildDateLengthData, (uint8*)&buildDateLengthData + sizeof(buildDateLengthData));
    data.insert(data.end(), buildDate.cString(), buildDate.cString() + buildDate.length());
    return writeFile(data, filePath);
}
