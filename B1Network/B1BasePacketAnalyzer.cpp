//
// B1BasePacketAnalyzer.cpp
//
// Library: B1Network
// Package: B1Network
// Module:  B1Network
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1Network.h"
#include "B1Network/B1BasePacketAnalyzer.h"
#include "B1Network/B1BaseComponent.h"

using namespace BnD;

B1BasePacketAnalyzer::B1BasePacketAnalyzer(size_t defaultBufferSize)
{
    _recvdBuffer.reserve(defaultBufferSize);
}

B1BasePacketAnalyzer::~B1BasePacketAnalyzer()
{
}

bool B1BasePacketAnalyzer::analyzeData(const uint8* data, size_t size)
{
    if (_recvdBuffer.empty() != true) {
        if (size > 0) {
            _recvdBuffer.insert(_recvdBuffer.end(), data, data + size);
        }
        data = &_recvdBuffer[0];
        size = _recvdBuffer.size();
    }
    if (size < 1 || NULL == data) {
        B1LOG("invalid data size or empty data: size[%d]", size);
        assert(false);
        return false;
    }
    if (size > CONSTS_MAX_DATA_SIZE) {
        B1LOG("data size is TOO BIG: limit_size[%u], recvd_size[%u]", CONSTS_MAX_DATA_SIZE, size);
        assert(false);
        return false;
    }
    size_t pos = 0;
    auto result = implAnalyzeData((uint8*)data, size, &pos);
    if (ANALYZE_RESULT_FAIL == result) {
        B1LOG("analyze data failed: size[%d], pos[%d]", size, pos);
        return false;
    }
    if (ANALYZE_RESULT_SUCCESS == result) {
        if (pos == size) {
            _recvdBuffer.clear();
        }
        else {
            if (size < pos) {
                B1LOG("invalid packet recved: size[%d], pos[%d]", size, pos);
                assert(false);
                return false;
            }
            std::vector<uint8> newBuffer((uint8*)data + pos, (uint8*)data + size);
            _recvdBuffer.swap(newBuffer);
            analyzeData(NULL, 0);
        }
    }
    else if (ANALYZE_RESULT_NOT_ENOUTH_DATA == result) {
        std::vector<uint8> newBuffer((uint8*)data, (uint8*)data + size);
        _recvdBuffer.swap(newBuffer);
    }
    return true;
}

void B1BasePacketAnalyzer::clearBuffer()
{
    _recvdBuffer.clear();
}
