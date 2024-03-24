//
// B1RedisDirectPacketMaker.cpp
//
// Library: B1Redic
// Package: B1Redic
// Module:  B1Redic
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1Redic.h"
#include "B1RedisDirectPacketMaker.h"

using namespace BnD;

B1RedisDirectPacketMaker::B1RedisDirectPacketMaker()
{
}

B1RedisDirectPacketMaker::~B1RedisDirectPacketMaker()
{
}

void B1RedisDirectPacketMaker::setBulkStringCommand(const B1String& command, size_t argSize, std::vector<uint8>* data)
{
    B1String temp(B1String::formatAs("*%d\r\n$%d\r\n%s\r\n", argSize + 1, command.length(), command.cString()));
    data->insert(data->end(), temp.cString(), temp.cString() + temp.length());
}

void B1RedisDirectPacketMaker::appendBulkStringCommand(const B1String& arg, std::vector<uint8>* data)
{
    B1String temp(B1String::formatAs("$%d\r\n%s\r\n", arg.length(), arg.cString()));
    data->insert(data->end(), temp.cString(), temp.cString() + temp.length());
}

std::vector<uint8> B1RedisDirectPacketMaker::makeSimpleStringCommand(const B1String& command)
{
    std::vector<uint8> data;
    data.reserve(command.length() + 2);
    data.insert(data.end(), command.cString(), command.cString() + command.length());
    data.push_back('\r');
    data.push_back('\n');
    return data;
}

std::vector<uint8> B1RedisDirectPacketMaker::makeDataRedisKey(const B1String& command, const B1String& key)
{
    std::vector<uint8> data;
    const size_t argsSize = 1;
    data.reserve((argsSize + 1) * CONSTS_STRING_DEFAULT_LENGTH);
    setBulkStringCommand(command, argsSize, &data);
    appendBulkStringCommand(key, &data);
    return data;
}

std::vector<uint8> B1RedisDirectPacketMaker::makeDataRedisKeyArg(const B1String& command, const B1String& arg1, const B1String& arg2)
{
    std::vector<uint8> data;
    const size_t argsSize = 2;
    data.reserve((argsSize + 1) * CONSTS_STRING_DEFAULT_LENGTH);
    setBulkStringCommand(command, argsSize, &data);
    appendBulkStringCommand(arg1, &data);
    appendBulkStringCommand(arg2, &data);
    return data;
}

std::vector<uint8> B1RedisDirectPacketMaker::makeDataPing()
{
    return makeSimpleStringCommand("PING");
}

std::vector<uint8> B1RedisDirectPacketMaker::makeDataTime()
{
    return makeSimpleStringCommand("TIME");
}

std::vector<uint8> B1RedisDirectPacketMaker::makeDataSelectDB(int32 db)
{
    return makeSimpleStringCommand(B1String::formatAs("SELECT %d", db));
}

std::vector<uint8> B1RedisDirectPacketMaker::makeDataExists(const B1String& key)
{
    return makeSimpleStringCommand(B1String::formatAs("EXISTS %s", key.cString()));
}

std::vector<uint8> B1RedisDirectPacketMaker::makeDataHmget(const B1String& key, const std::vector<B1String>& fields)
{
    std::vector<uint8> data;
    const size_t argsSize = 1 + fields.size();
    data.reserve((argsSize + 1) * CONSTS_STRING_DEFAULT_LENGTH);
    setBulkStringCommand("HMGET", argsSize, &data);
    appendBulkStringCommand(key, &data);
    for (const auto& field : fields) {
        appendBulkStringCommand(field, &data);
    }
    return data;
}

std::vector<uint8> B1RedisDirectPacketMaker::makeDataHgetall(const B1String& key)
{
    return makeDataRedisKey("HGETALL", key);
}

std::vector<uint8> B1RedisDirectPacketMaker::makeDataSmembers(const B1String& key)
{
    return makeDataRedisKey("SMEMBERS", key);
}

std::vector<uint8> B1RedisDirectPacketMaker::makeDataIncr(const B1String& key)
{
    return makeDataRedisKey("INCR", key);
}

std::vector<uint8> B1RedisDirectPacketMaker::makeDataHmset(const B1String& key, const std::map<B1String, B1String>& fieldValues)
{
    std::vector<uint8> data;
    const size_t argsSize = 1 + fieldValues.size() * 2;
    data.reserve((argsSize + 1) * CONSTS_STRING_DEFAULT_LENGTH);
    setBulkStringCommand("HMSET", argsSize, &data);
    appendBulkStringCommand(key, &data);
    for (const auto& fieldValuesPair : fieldValues) {
        appendBulkStringCommand(fieldValuesPair.first, &data);
        appendBulkStringCommand(fieldValuesPair.second, &data);
    }
    return data;
}

std::vector<uint8> B1RedisDirectPacketMaker::makeDataPublish(const B1String& channel, const B1String& message)
{
    return makeDataRedisKeyArg("PUBLISH", channel, message);
}

std::vector<uint8> B1RedisDirectPacketMaker::makeDataSubscribe(const std::set<B1String>& channels)
{
    std::vector<uint8> data;
    const size_t argsSize = channels.size();
    data.reserve((argsSize + 1) * CONSTS_STRING_DEFAULT_LENGTH);
    setBulkStringCommand("SUBSCRIBE", argsSize, &data);
    for (const auto& channel : channels) {
        appendBulkStringCommand(channel, &data);
    }
    return data;
}

std::vector<uint8> B1RedisDirectPacketMaker::makeDataLindex(const B1String& key, int32 index)
{
    return makeDataRedisKeyArg("LINDEX", key, B1String::formatAs("%d", index));
}

std::vector<uint8> B1RedisDirectPacketMaker::makeDataLpop(const B1String& key)
{
    return makeDataRedisKey("LPOP", key);
}

std::vector<uint8> B1RedisDirectPacketMaker::makeDataHdel(const B1String& key, const B1String& field)
{
    return makeDataRedisKeyArg("HDEL", key, field);
}

std::vector<uint8> B1RedisDirectPacketMaker::makeDataDel(const B1String& key)
{
    return makeDataRedisKey("DEL", key);
}

std::vector<uint8> B1RedisDirectPacketMaker::makeDataSadd(const B1String& key, const B1String& value)
{
    return makeDataRedisKeyArg("SADD", key, value);
}

std::vector<uint8> B1RedisDirectPacketMaker::makeDataSrem(const B1String& key, const B1String& value)
{
    return makeDataRedisKeyArg("SREM", key, value);
}

std::vector<uint8> B1RedisDirectPacketMaker::makeDataRpush(const B1String& key, const B1String& value)
{
    return makeDataRedisKeyArg("RPUSH", key, value);
}

std::vector<uint8> B1RedisDirectPacketMaker::makeDataArgs(const std::vector<B1String>& args)
{
    std::vector<uint8> data;
    if (args.empty()) {
        return data;
    }
    else if (args.size() == 1) {
        return makeSimpleStringCommand(args.front());
    }
    else {
        const size_t argsSize = args.size() - 1;
        data.reserve(argsSize * CONSTS_STRING_DEFAULT_LENGTH);
        setBulkStringCommand(args.front(), argsSize, &data);
        for (size_t i = 1; i < args.size(); ++i) {
            appendBulkStringCommand(args[i], &data);
        }
        return data;
    }
}
