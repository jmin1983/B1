//
// B1StringUtil.cpp
//
// Library: B1Base
// Package: B1Base
// Module:  B1Base
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1Base.h"
#include "B1StringUtil.h"
#include "B1Archive.h"

using namespace BnD;

B1String B1StringUtil::lastPathComponent(const B1String &path)
{
    uint32 i = path.findLastOf("/\\");
    if (i == B1String::NPOS) {
        return path.copy();
    }

    uint32 length = path.length();
    if (length == 1) {
        return path.copy();
    }
    else if (i < length - 1) {
        return path.substring(i + 1);
    }
    else {
        i = path.findLastOf("/\\", length - 2);
        return path.substring(i + 1, length - 2 - (i + 1) + 1);
    }
}

void B1StringUtil::removeLastPathComponent(B1String *path)
{
    removeLastPathSeparator(path);
    B1String s = lastPathComponent(*path);
    if (0 < s.length() && s != *path) {
        path->resize(path->length() - s.length());
        removeLastPathSeparator(path);
    }
}

void B1StringUtil::removeLastPathSeparator(B1String *path)
{
    uint32 i = path->findLastOf("/\\");
    if (i != B1String::NPOS) {
        uint32 length = path->length();
        if (0 < i && i == length - 1) {
            path->resize(length - 1);
        }
    }
}

void B1StringUtil::binaryToHexaString(const std::vector<uint8>& source, B1String* result)
{
    result->clear();
    for (const auto d : source) {
        result->appendf("%02X", d);
    }
}

void B1StringUtil::hexaStringToBinary(const B1String& source, std::vector<uint8>* result)
{
    uint32 sourceLength = source.length();
    uint32 resultLength = sourceLength / 2;
    result->resize(resultLength);
    int32 index = 0;
    for (uint32 i = 0; i < sourceLength; i += 2) {
        auto upper = source.substring(i, 1);
        auto lower = source.substring(i + 1, 1);
        result->at(index++) = static_cast<uint8>((strtol(upper.cString(), NULL, 16) << 4) + strtol(lower.cString(), NULL, 16));
    }
}

bool B1StringUtil::vectorIntToString(const std::vector<int32>& source, B1String* result)
{
    B1Archive archive;
    archive.writeData("B1VectorInt32", source);
    return archive.toString(result);
}

bool B1StringUtil::stringToVectorInt(const B1String& source, std::vector<int32>* result)
{
    B1Archive archive;
    archive.fromString(source);
    std::vector<int32>().swap(*result);
    return archive.readData("B1VectorInt32", result);
}
