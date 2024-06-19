//
// B1RedisDirectClientReadSession.cpp
//
// Library: B1Redic
// Package: B1Redic
// Module:  B1Redic
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1Redic.h"
#include "B1RedisDirectClientReadSession.h"
#include "B1RedisDirectClient.h"
#include "B1RedisDirectClientListener.h"
#include "B1RedisDirectPacketMaker.h"

#include <B1Base/B1TickUtil.h>
#include <B1Base/B1Condition.h>
#include <B1Base/B1Lock.h>
#include <B1Base/B1Thread.h>
#include <B1Base/B1Time.h>

using namespace BnD;

B1RedisDirectClientReadSession::B1RedisDirectClientReadSession(B1ClientSocket* clientSocket, B1BaseClientSessionListener* listener, B1RedisDirectClient* owner)
    : B1RedisDirectClientSession(clientSocket, listener, owner)
    , _inited(false)
    , _recvedBuffer()
    , _adjustSeconds(0)
    , _wait(new B1Condition())
    , _lock(new B1Lock())
{
}

B1RedisDirectClientReadSession::~B1RedisDirectClientReadSession()
{
}

void B1RedisDirectClientReadSession::implOnRecvRedisSimpleStrings(const B1String& string)
{
    if (_inited != true) {
        if (string.caseInsensitiveCompare("ok")) {
            B1LOG("redis db selected");
            _inited = true;
        }
        return;
    }
    if (_recvedBuffer.empty() != true) {
        if (string.caseInsensitiveCompare("pong")) {
            return;
        }
        B1LOG("_recvedBuffer is not empty, but recved simple_string:[%s]", string.cString());
        assert(false);
        return;
    }
    _recvedBuffer.clear();
    _recvedBuffer.push_back(string.copy());
    _wait->signal();
}

void B1RedisDirectClientReadSession::implOnRecvRedisErrors(const B1String& message)
{
    B1RedisDirectClientSession::implOnRecvRedisErrors(message);
    if (_inited != true) {
        return;
    }
    if (_recvedBuffer.empty() != true) {
        B1LOG("_recvedBuffer is not empty, but recved error:[%s]", message.cString());
        assert(false);
        return;
    }
    _recvedBuffer.clear();
    _wait->signal();
}

void B1RedisDirectClientReadSession::implOnRecvRedisIntegers(const B1String& int64String)
{
    if (_inited != true) {
        return;
    }
    if (_recvedBuffer.empty() != true) {
        B1LOG("_recvedBuffer is not empty, but recved integer:[%s]", int64String.cString());
        assert(false);
        return;
    }
    _recvedBuffer.clear();
    _recvedBuffer.push_back(int64String.copy());
    _wait->signal();
}

void B1RedisDirectClientReadSession::implOnRecvRedisBulkStrings(const B1String& string)
{
    if (_inited != true) {
        if (string.caseInsensitiveCompare("ok")) {
            B1LOG("redis db selected");
            _inited = true;
        }
        return;
    }
    if (_recvedBuffer.empty() != true) {
        B1LOG("_recvedBuffer is not empty, but recved bulk_string:[%s]", string.cString());
        assert(false);
        return;
    }
    _recvedBuffer.clear();
    _recvedBuffer.push_back(string.copy());
    _wait->signal();
}

void B1RedisDirectClientReadSession::implOnRecvRedisArrays(const std::list<B1String>& strings)
{
    if (_inited != true) {
        if (strings.empty() != true && strings.front().caseInsensitiveCompare("ok")) {
            B1LOG("redis db selected");
            _inited = true;
        }
        return;
    }
    if (_recvedBuffer.empty() != true) {
        B1String log("_recvedBuffer is not empty, but recved array:");
        for (const auto& string : strings) {
            log.appendf("[%s]", string.cString());
        }
        B1LOG("%s", log.cString());
        assert(false);
        return;
    }
    _recvedBuffer.clear();
    for (const auto& string : strings) {
        _recvedBuffer.push_back(string.copy());
    }
    _wait->signal();
}

void B1RedisDirectClientReadSession::implOnDisconnected(int32 reason)
{
    _inited = false;
    _wait->reset();
    if (auto listener = owner()->listener()) {
        listener->onRedisDirectReaderDisconnected();
    }
    B1RedisDirectClientSession::implOnDisconnected(reason);
}

void B1RedisDirectClientReadSession::implProcessConnected(bool firstConnectedProcess)
{
    B1RedisDirectClientSession::implProcessConnected(firstConnectedProcess);
    if (firstConnectedProcess) {
        if (auto listener = owner()->listener()) {
            listener->onRedisDirectReaderConnected();
        }
    }
}

void B1RedisDirectClientReadSession::implCheckAlive()
{
    if (_lock->tryLock()) {
        if (setWaitData() != true) {
            _lock->unlock();
            disconnect();
            return;
        }
        {
            auto data = packetMaker()->makeDataPing();
            if (writeData(data) != true) {
                _lock->unlock();
                disconnect();
                return;
            }
        }
        {
            std::list<B1String> data;
            if (waitData(&data) != true) {
                _lock->unlock();
                disconnect();
                return;
            }
            _lock->unlock();
            for (const auto& d : data) {
                checkAliveString(d);
            }
        }
    }
    else {
        _aliveCheckCount = 0;
    }
    checkAliveCount();
}

bool B1RedisDirectClientReadSession::setWaitData()
{
    if (_wait->isSignaled()) {
        B1LOG("already waiting data!");
        assert(false);
        return false;
    }
    return true;
}

bool B1RedisDirectClientReadSession::waitData(std::list<B1String>* data)
{
    if (_wait->wait(CONSTS_WAIT_DATA_TIME_OUT) != true) {
        B1LOG("waiting data timed_out");
        return false;
    }
    if (data) {
        data->swap(_recvedBuffer);
    }
    return true;
}

bool B1RedisDirectClientReadSession::scan(const B1String& cursor, const B1String& pattern, std::list<B1String>* out, int32 count)
{
    _lock->lock();
    if (setWaitData() != true) {
        _lock->unlock();
        disconnect();
        return false;
    }
    {
        auto data = packetMaker()->makeDataScan(cursor, pattern, count);
        if (writeData(data) != true) {
            _lock->unlock();
            disconnect();
            return false;
        }
    }
    {
        std::list<B1String> data;
        if (waitData(&data) != true) {
            _lock->unlock();
            disconnect();
            return false;
        }
        _lock->unlock();
        if (data.empty()) {
            assert(false);
            return true;
        }
        if (data.size() > 1) {
            out->insert(out->end(), ++data.begin(), data.end());
        }
        if (data.front() != "0") {
            scan(data.front(), pattern, out, count);
        }
    }
    return true;
}

bool B1RedisDirectClientReadSession::exists(const B1String& key)
{
    _lock->lock();
    if (setWaitData() != true) {
        _lock->unlock();
        disconnect();
        return false;
    }
    {
        auto data = packetMaker()->makeDataExists(key);
        if (writeData(data) != true) {
            _lock->unlock();
            disconnect();
            return false;
        }
    }
    {
        std::list<B1String> data;
        if (waitData(&data) != true) {
            _lock->unlock();
            disconnect();
            return false;
        }
        _lock->unlock();
        if (data.empty()) {
            return false;
        }
        try {
            return data.front().toInt32() > 0;
        }
        catch (...) {}
    }
    return false;
}

bool B1RedisDirectClientReadSession::hmget(const B1String& key, const std::vector<B1String>& fields, std::vector<B1String>* out)
{
    out->resize(fields.size());
    _lock->lock();
    if (setWaitData() != true) {
        _lock->unlock();
        disconnect();
        return false;
    }
    {
        auto data = packetMaker()->makeDataHmget(key, fields);
        if (writeData(data) != true) {
            _lock->unlock();
            disconnect();
            return false;
        }
    }
    {
        std::list<B1String> data;
        if (waitData(&data) != true) {
            _lock->unlock();
            disconnect();
            return false;
        }
        _lock->unlock();
        size_t index = 0;
        for (auto& d : data) {
            if (index >= out->size()) {
                break;
            }
            out->at(index++) = std::move(d);
        }
    }
    return true;
}

B1String B1RedisDirectClientReadSession::hmget(const B1String& key, const B1String& field)
{
    std::vector<B1String> fields(1);
    fields[0] = field.copy();
    std::vector<B1String> out;
    if (hmget(key, fields, &out) != true) {
        return "";
    }
    return out.empty() != true ? out.front() : "";
}

bool B1RedisDirectClientReadSession::hgetall(const B1String& key, std::map<B1String, B1String>* out)
{
    _lock->lock();
    if (setWaitData() != true) {
        _lock->unlock();
        disconnect();
        return false;
    }
    {
        auto data = packetMaker()->makeDataHgetall(key);
        if (writeData(data) != true) {
            _lock->unlock();
            disconnect();
            return false;
        }
    }
    {
        std::list<B1String> data;
        if (waitData(&data) != true) {
            _lock->unlock();
            disconnect();
            return false;
        }
        _lock->unlock();
        for (std::list<B1String>::iterator itr(data.begin()); itr != data.end(); ++itr) {
            B1String& field = *itr++;
            out->insert(std::make_pair(std::move(field), std::move(*itr)));
        }
    }
    return true;
}

bool B1RedisDirectClientReadSession::scan(const B1String& pattern, std::list<B1String>* out, int32 count)
{
    out->clear();
    return scan("0", pattern, out, count);
}

bool B1RedisDirectClientReadSession::smembers(const B1String& key, std::list<B1String>* out)
{
    _lock->lock();
    if (setWaitData() != true) {
        _lock->unlock();
        disconnect();
        return false;
    }
    {
        auto data = packetMaker()->makeDataSmembers(key);
        if (writeData(data) != true) {
            _lock->unlock();
            disconnect();
            return false;
        }
    }
    {
        std::list<B1String> data;
        if (waitData(&data) != true) {
            _lock->unlock();
            disconnect();
            return false;
        }
        _lock->unlock();
        out->swap(data);
    }
    return true;
}

bool B1RedisDirectClientReadSession::incr(const B1String& key, int64* result)
{
    _lock->lock();
    if (setWaitData() != true) {
        _lock->unlock();
        disconnect();
        return false;
    }
    {
        auto data = packetMaker()->makeDataIncr(key);
        if (writeData(data) != true) {
            _lock->unlock();
            disconnect();
            return false;
        }
    }
    {
        std::list<B1String> data;
        if (waitData(&data) != true) {
            _lock->unlock();
            disconnect();
            return false;
        }
        _lock->unlock();
        if (data.size() != 1) {
            return false;
        }
        try {
            *result = data.front().toInt64();
        }
        catch (...) {
            return false;
        }
    }
    return true;
}

bool B1RedisDirectClientReadSession::time(uint64* seconds, uint32* microseconds, bool ignoreAdjustSecond)
{
    _lock->lock();
    if (setWaitData() != true) {
        _lock->unlock();
        disconnect();
        return false;
    }
    {
        auto data = packetMaker()->makeDataTime();
        if (writeData(data) != true) {
            _lock->unlock();
            disconnect();
            return false;
        }
    }
    {
        std::list<B1String> data;
        if (waitData(&data) != true) {
            _lock->unlock();
            disconnect();
            return false;
        }
        _lock->unlock();
        if (data.size() != 2) {
            return false;
        }
        if (seconds) {
            try {
                *seconds = data.front().toUint64();
                if (ignoreAdjustSecond != true) {
                    *seconds += _adjustSeconds;
                }
            }
            catch (...) {
                return false;
            }
        }
        if (microseconds) {
            try {
                *microseconds = data.back().toUint32();
            }
            catch (...) {
                return false;
            }
        }
    }
    return true;
}

void B1RedisDirectClientReadSession::time(B1String* result, bool useTimeLength17)
{
    uint64 seconds = 0;
    uint32 microseconds = 0;
    if (time(&seconds, &microseconds) != true) {
        B1LOG("can not get time from Server");
        *result = B1Time::currentTimeInMilliseconds();
        if (useTimeLength17) {
            result->append("0");
        }
    }
    else {
        B1Time t(seconds);
        if (useTimeLength17) {
            result->format("%04d%02d%02d%02d%02d%02d%03d", t.year(), t.month(), t.day(), t.hour(), t.minute(), t.second(), static_cast<uint32>(microseconds / 1000));
        }
        else {
            result->format("%04d%02d%02d%02d%02d%02d%02d", t.year(), t.month(), t.day(), t.hour(), t.minute(), t.second(), static_cast<uint32>(microseconds / 10000));
        }
    }
}

void B1RedisDirectClientReadSession::timeMMDDHHmmSSnnnn(B1String* result)
{
    uint64 seconds = 0;
    uint32 microseconds = 0;
    if (time(&seconds, &microseconds) != true) {
        B1LOG("Can not get time from Server");
        *result = B1Time::currentTimeMMDDHHmmSSnnnn();
    }
    else {
        B1Time t(seconds);
        result->format("%02d%02d%02d%02d%02d%04d", t.month(), t.day(), t.hour(), t.minute(), t.second(), static_cast<uint32>(microseconds / 100));
    }
}

bool B1RedisDirectClientReadSession::shutdownRemoteRedis()
{
    std::vector<B1String> args(1);
    args[0] = "SHUTDOWN";
    _lock->lock();
    auto data = packetMaker()->makeDataArgs(args);
    if (writeData(data) != true) {
        _lock->unlock();
        disconnect();
        return false;
    }
    _lock->unlock();
    return true;
}

bool B1RedisDirectClientReadSession::blockedCommand(const std::vector<B1String>& args, std::list<B1String>* out)
{
    //  DO NOT USE "SHUTDOWN" command. see shutdownRemoteRedis()
    _lock->lock();
    if (setWaitData() != true) {
        _lock->unlock();
        disconnect();
        return false;
    }
    {
        auto data = packetMaker()->makeDataArgs(args);
        if (writeData(data) != true) {
            _lock->unlock();
            disconnect();
            return false;
        }
    }
    {
        std::list<B1String> data;
        if (waitData(&data) != true) {
            _lock->unlock();
            disconnect();
            return false;
        }
        _lock->unlock();
        if (out) {
            out->swap(data);
        }
    }
    return true;
}

void B1RedisDirectClientReadSession::blockedCommand(const std::vector<std::vector<B1String> >& cmds)
{
    _lock->lock();
    for (const auto& args : cmds) {
        if (setWaitData() != true) {
            _lock->unlock();
            disconnect();
            return;
        }
        {
            auto data =  packetMaker()->makeDataArgs(args);
            if (writeData(data) != true) {
                _lock->unlock();
                disconnect();
                return;
            }
        }
        if (waitData() != true) {
            _lock->unlock();
            disconnect();
            return;
        }
    }
    _lock->unlock();
}
