//
// B1RedisDirectPacketAnalyzer.h
//
// Library: B1Redic
// Package: B1Redic
// Module:  B1Redic
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1REDIC_REDIS_DIRECT_PACKET_ANALYZER_H
#define _B1REDIC_REDIS_DIRECT_PACKET_ANALYZER_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <B1Network/B1BasePacketAnalyzer.h>

#include <list>

namespace BnD {
    class B1RedisDirectPacketAnalyzer : public B1BasePacketAnalyzer {
    protected:
        virtual void implOnRecvRedisSimpleStrings(const B1String& string) {}
        virtual void implOnRecvRedisErrors(const B1String& message) {}
        virtual void implOnRecvRedisIntegers(const B1String& int64String) {}        //  just using string. actually it is int64 range value.
        virtual void implOnRecvRedisBulkStrings(const B1String& string) {}
        virtual void implOnRecvRedisArrays(const std::list<B1String>& strings) {}   //  just set as string. ignore the resp_type. empty if null_array.
    protected:
        ANALYZE_RESULT implAnalyzeData(uint8* data, size_t size, size_t* pos) final;
    protected:
        bool getValueLengthFromData(uint8* data, size_t size, size_t* length) const;
        ANALYZE_RESULT getRedisString(uint8* data, size_t size, B1String* string) const;
        ANALYZE_RESULT getRedisBulkString(uint8* data, size_t size, B1String* string, size_t* totalLength) const;
        ANALYZE_RESULT getRedisArrayString(uint8* data, size_t size, std::list<B1String>* strings, size_t* totalLength) const;
        ANALYZE_RESULT analyzeRedisSimpleStrings(uint8* data, size_t size, size_t* pos);
        ANALYZE_RESULT analyzeRedisErrors(uint8* data, size_t size, size_t* pos);
        ANALYZE_RESULT analyzeRedisIntegers(uint8* data, size_t size, size_t* pos);
        ANALYZE_RESULT analyzeRedisBulkStrings(uint8* data, size_t size, size_t* pos);
        ANALYZE_RESULT analyzeRedisArrays(uint8* data, size_t size, size_t* pos);
    };
}   //  !BnD

#endif  // !_B1REDIC_REDIS_DIRECT_PACKET_ANALYZER_H
