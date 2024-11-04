//
// B1RedisDirectPacketAnalyzer.cpp
//
// Library: B1Redic
// Package: B1Redic
// Module:  B1Redic
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1Redic.h"
#include "B1RedisDirectPacketAnalyzer.h"
#include "B1RedisDirectProtocol.h"

using namespace BnD;

B1BasePacketAnalyzer::ANALYZE_RESULT B1RedisDirectPacketAnalyzer::implAnalyzeData(uint8* data, size_t size, size_t* pos)
{
    if (size < 3) {
        return ANALYZE_RESULT_NOT_ENOUTH_DATA;
    }
    const uint8 prefix = data[0];
    (*pos)++;
    switch (prefix) {
        case B1RedisDirectProtocol::RESP_TYPE_SIMPLE_STRINGS:
            return analyzeRedisSimpleStrings(data + (*pos), size - (*pos), pos);
        case B1RedisDirectProtocol::RESP_TYPE_ERRORS:
            return analyzeRedisErrors(data + (*pos), size - (*pos), pos);
        case B1RedisDirectProtocol::RESP_TYPE_INTEGERS:
            return analyzeRedisIntegers(data + (*pos), size - (*pos), pos);
        case B1RedisDirectProtocol::RESP_TYPE_BULK_STRINGS:
            return analyzeRedisBulkStrings(data + (*pos), size - (*pos), pos);
        case B1RedisDirectProtocol::RESP_TYPE_ARRAYS:
            return analyzeRedisArrays(data + (*pos), size - (*pos), pos);
        default:
            break;
    }
    B1LOG("Unknown type: prefix[%c], size[%d], recvdBuffer_size[%d]", prefix, size, _recvdBuffer.size());
    assert(false);
    return ANALYZE_RESULT_FAIL;
}

bool B1RedisDirectPacketAnalyzer::getValueLengthFromData(uint8* data, size_t size, size_t* length) const
{
    for (size_t i = 1; i < size; ++i) {
        if (data[i - 1] == '\r' && data[i] == '\n') {
            *length = i - 1;
            return true;
        }
    }
    return false;
}

B1RedisDirectPacketAnalyzer::ANALYZE_RESULT B1RedisDirectPacketAnalyzer::getRedisString(uint8* data, size_t size, B1String* string) const
{
    size_t length = 0;
    if (getValueLengthFromData(data, size, &length) != true) {
        return ANALYZE_RESULT_NOT_ENOUTH_DATA;
    }
    std::vector<char> buffer(length + 1, 0);
    memcpy(&buffer[0], data, length);
    string->from(&buffer[0]);
    return ANALYZE_RESULT_SUCCESS;
}

B1RedisDirectPacketAnalyzer::ANALYZE_RESULT B1RedisDirectPacketAnalyzer::getRedisBulkString(uint8* data, size_t size, B1String* string, size_t* totalLength) const
{
    int32 stringLength = 0; //  -1 if null.
    {
        B1String string;
        auto result = getRedisString(data, size, &string);
        if (result != ANALYZE_RESULT_SUCCESS) {
            return result;
        }
        try {
            stringLength = string.toInt32();
        }
        catch (...) {
            return ANALYZE_RESULT_FAIL;
        }
        if (stringLength > 0) {
            if (static_cast<size_t>(stringLength) > size) {
                return ANALYZE_RESULT_NOT_ENOUTH_DATA;
            }
        }
        (*totalLength) += (string.length() + 2);
    }
    if (stringLength < 0) {
        string->from("");   //  The client library API should not return an empty string, but a nil object... but... just returns empty_string for convenience.
    }
    else {  //  do not use getValueLengthFromData() for bulk_string.
        std::vector<char> buffer(stringLength + 1, 0);
        memcpy(&buffer[0], data + *totalLength, stringLength);
        string->from(&buffer[0]);
        if (data[*totalLength + stringLength] == '\r' && data[*totalLength + stringLength + 1] == '\n') {
            (*totalLength) += (stringLength + 2);
        }
        else if (data[*totalLength + stringLength] == '\r' || data[*totalLength + stringLength] == '\n') {
            (*totalLength) += (stringLength + 1);
        }
        else {
            (*totalLength) += stringLength;
        }
    }
    return ANALYZE_RESULT_SUCCESS;
}

B1RedisDirectPacketAnalyzer::ANALYZE_RESULT B1RedisDirectPacketAnalyzer::getRedisArrayString(uint8* data, size_t size, std::list<B1String>* strings, size_t* totalLength) const
{
    int32 arrayLength = 0;  //  -1 if null.
    {
        B1String string;
        auto result = getRedisString(data, size, &string);
        if (result != ANALYZE_RESULT_SUCCESS) {
            return result;
        }
        try {
            arrayLength = string.toInt32();
        }
        catch (...) {
            return ANALYZE_RESULT_FAIL;
        }
        (*totalLength) += (string.length() + 2);
    }
    if (arrayLength < 0) {
        return ANALYZE_RESULT_SUCCESS;
    }
    for (int32 i = 0; i < arrayLength; ++i) {
        if (*totalLength >= size) {
            return ANALYZE_RESULT_NOT_ENOUTH_DATA;
        }
        const uint8 prefix = data[*totalLength];
        (*totalLength)++;
        switch (prefix) {
            case B1RedisDirectProtocol::RESP_TYPE_SIMPLE_STRINGS:
            case B1RedisDirectProtocol::RESP_TYPE_ERRORS:
            case B1RedisDirectProtocol::RESP_TYPE_INTEGERS: {
                B1String string;
                auto result = getRedisString(data + (*totalLength), size - (*totalLength), &string);
                if (result != ANALYZE_RESULT_SUCCESS) {
                    return result;
                }
                (*totalLength) += (string.length() + 2);
                strings->push_back(std::move(string));
                break;
            }
            case B1RedisDirectProtocol::RESP_TYPE_BULK_STRINGS: {
                B1String string;
                size_t length = 0;
                auto result = getRedisBulkString(data + (*totalLength), size - (*totalLength), &string, &length);
                if (result != ANALYZE_RESULT_SUCCESS) {
                    return result;
                }
                (*totalLength) += length;
                strings->push_back(std::move(string));
                break;
            }
            case B1RedisDirectProtocol::RESP_TYPE_ARRAYS: {
                size_t length = 0;
                auto result = getRedisArrayString(data + (*totalLength), size - (*totalLength), strings, &length);
                if (result != ANALYZE_RESULT_SUCCESS) {
                    return result;
                }
                (*totalLength) += length;
                break;
            }
            default:
                assert(false);
                return ANALYZE_RESULT_FAIL;
        }
    }
    return ANALYZE_RESULT_SUCCESS;
}

B1RedisDirectPacketAnalyzer::ANALYZE_RESULT B1RedisDirectPacketAnalyzer::analyzeRedisSimpleStrings(uint8* data, size_t size, size_t* pos)
{
    B1String string;
    {
        auto result = getRedisString(data, size, &string);
        if (result != ANALYZE_RESULT_SUCCESS) {
            return result;
        }
    }
    implOnRecvRedisSimpleStrings(string);
    (*pos) += (string.length() + 2);
    return ANALYZE_RESULT_SUCCESS;
}

B1RedisDirectPacketAnalyzer::ANALYZE_RESULT B1RedisDirectPacketAnalyzer::analyzeRedisErrors(uint8* data, size_t size, size_t* pos)
{
    B1String string;
    {
        auto result = getRedisString(data, size, &string);
        if (result != ANALYZE_RESULT_SUCCESS) {
            return result;
        }
    }
    implOnRecvRedisErrors(string);
    (*pos) += (string.length() + 2);
    return ANALYZE_RESULT_SUCCESS;
}

B1RedisDirectPacketAnalyzer::ANALYZE_RESULT B1RedisDirectPacketAnalyzer::analyzeRedisIntegers(uint8* data, size_t size, size_t* pos)
{
    B1String string;
    {
        auto result = getRedisString(data, size, &string);
        if (result != ANALYZE_RESULT_SUCCESS) {
            return result;
        }
    }
    implOnRecvRedisIntegers(string);
    (*pos) += (string.length() + 2);
    return ANALYZE_RESULT_SUCCESS;
}

B1RedisDirectPacketAnalyzer::ANALYZE_RESULT B1RedisDirectPacketAnalyzer::analyzeRedisBulkStrings(uint8* data, size_t size, size_t* pos)
{
    B1String string;
    size_t totalLength = 0; //  must do initialization.
    {
        auto result = getRedisBulkString(data, size, &string, &totalLength);
        if (result != ANALYZE_RESULT_SUCCESS) {
            return result;
        }
    }
    implOnRecvRedisBulkStrings(string);
    (*pos) += totalLength;
    return ANALYZE_RESULT_SUCCESS;
}

B1RedisDirectPacketAnalyzer::ANALYZE_RESULT B1RedisDirectPacketAnalyzer::analyzeRedisArrays(uint8* data, size_t size, size_t* pos)
{
    std::list<B1String> strings;
    size_t totalLength = 0; //  must do initialization.
    {
        auto result = getRedisArrayString(data, size, &strings, &totalLength);
        if (result != ANALYZE_RESULT_SUCCESS) {
            return result;
        }
    }
    implOnRecvRedisArrays(strings);
    (*pos) += totalLength;
    return ANALYZE_RESULT_SUCCESS;
}
