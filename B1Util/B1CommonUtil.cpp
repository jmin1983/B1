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
    if (data.size() <= limitSize) {
        return std::vector<std::vector<uint8> >(1, std::move(data));
    }
    const size_t size = data.size() / limitSize + 1;
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
