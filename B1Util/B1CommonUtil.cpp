//
// B1CommonUtil.cpp
//
// Library: B1Util
// Package: B1Util
// Module:  B1Util
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1Util.h"
#include "B1CommonUtil.h"
#include "B1FileUtil.h"

#include <B1Base/B1SystemUtil.h>

#include <random>

using namespace BnD;

int32 B1CommonUtil::isInRange(float64 l, float64 r, float64 range)
{
    if (l > r) {
        if ((l - r) > range) {
            return -1;
        }
    }
    else if (r > l) {
        if ((r - l) > range) {
            return 1;
        }
    }
    return 0;
}

auto B1CommonUtil::splitData(std::vector<uint8>&& data, size_t limitSize) -> std::vector<std::vector<uint8> >
{
    if (limitSize < 1 || data.size() <= limitSize) {    //  no split if limitSize is 0.
        return std::vector<std::vector<uint8> >(1, std::move(data));
    }
    const size_t size = (data.size() + limitSize - 1) / limitSize;
    std::vector<std::vector<uint8> > result;
    result.reserve(size);
    for (size_t i = 0; i < size; ++i) {
        if (size == i + 1) {
            result.push_back(std::vector<uint8>(std::make_move_iterator(data.begin() + limitSize * i), std::make_move_iterator(data.end())));
        }
        else {
            result.push_back(std::vector<uint8>(std::make_move_iterator(data.begin() + limitSize * i), std::make_move_iterator(data.begin() + limitSize * (i + 1))));
        }
    }
    return result;
}

B1String B1CommonUtil::generateUUID()
{
    std::random_device randomDevice;    //  OS random source.
    uint8 bytes[16];
    for (int32 i = 0; i < 16; i += 4) {
        const uint32 value = randomDevice();
        bytes[i + 0] = static_cast<uint8>(value >> 24);
        bytes[i + 1] = static_cast<uint8>(value >> 16);
        bytes[i + 2] = static_cast<uint8>(value >> 8);
        bytes[i + 3] = static_cast<uint8>(value);
    }
    bytes[6] = static_cast<uint8>((bytes[6] & 0x0F) | 0x40);    //  version 4.
    bytes[8] = static_cast<uint8>((bytes[8] & 0x3F) | 0x80);    //  variant RFC 4122.
    static const char* const s_hexDigits = "0123456789abcdef";
    std::string result;
    result.reserve(36);
    for (int32 i = 0; i < 16; ++i) {
        if (4 == i || 6 == i || 8 == i || 10 == i) {
            result.push_back('-');
        }
        result.push_back(s_hexDigits[bytes[i] >> 4]);
        result.push_back(s_hexDigits[bytes[i] & 0x0F]);
    }
    return B1String(std::move(result));
}
