//
// B1RedisDirectPacketMaker.h
//
// Library: B1Redic
// Package: B1Redic
// Module:  B1Redic
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1REDIC_REDIS_DIRECT_PACKET_MAKER_H
#define _B1REDIC_REDIS_DIRECT_PACKET_MAKER_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <map>
#include <set>

namespace BnD {
    class B1RedisDirectPacketMaker {
    public:
        B1RedisDirectPacketMaker();
        virtual ~B1RedisDirectPacketMaker();
    protected:
        enum CONSTS {
            CONSTS_STRING_DEFAULT_LENGTH = 64,
        };
    protected:
        void setBulkStringCommand(const B1String& command, size_t argSize, std::vector<uint8>* data);
        void appendBulkStringCommand(const B1String& arg, std::vector<uint8>* data);
        std::vector<uint8> makeSimpleStringCommand(const B1String& command);
        std::vector<uint8> makeDataRedisKey(const B1String& command, const B1String& key);
        std::vector<uint8> makeDataRedisKeyArg(const B1String& command, const B1String& arg1, const B1String& arg2);
    public:
        std::vector<uint8> makeDataPing();
        std::vector<uint8> makeDataTime();
        std::vector<uint8> makeDataSelectDB(int32 db);
        std::vector<uint8> makeDataExists(const B1String& key);
        std::vector<uint8> makeDataHmget(const B1String& key, const std::vector<B1String>& fields);
        std::vector<uint8> makeDataHgetall(const B1String& key);
        std::vector<uint8> makeDataSmembers(const B1String& key);
        std::vector<uint8> makeDataIncr(const B1String& key);
        std::vector<uint8> makeDataHmset(const B1String& key, const std::map<B1String, B1String>& fieldValues);
        std::vector<uint8> makeDataPublish(const B1String& channel, const B1String& message);
        std::vector<uint8> makeDataSubscribe(const std::set<B1String>& channels);
        std::vector<uint8> makeDataLindex(const B1String& key, int32 index);
        std::vector<uint8> makeDataLpop(const B1String& key);
        std::vector<uint8> makeDataHdel(const B1String& key, const B1String& field);
        std::vector<uint8> makeDataDel(const B1String& key);
        std::vector<uint8> makeDataSadd(const B1String& key, const B1String& value);
        std::vector<uint8> makeDataSrem(const B1String& key, const B1String& value);
        std::vector<uint8> makeDataRpush(const B1String& key, const B1String& value);
        std::vector<uint8> makeDataArgs(const std::vector<B1String>& args);
    };
}   //  !BnD

#endif  // !_B1REDIC_REDIS_DIRECT_PACKET_MAKER_H
