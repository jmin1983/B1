//
// B1RedisDirectClientSession.cpp
//
// Library: B1Redic
// Package: B1Redic
// Module:  B1Redic
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1Redic.h"
#include "B1RedisDirectClientSession.h"
#include "B1RedisDirectClient.h"
#include "B1RedisDirectPacketMaker.h"
#include "B1RedisDirectProtocol.h"

#include <B1Base/B1TickUtil.h>
#include <B1Base/B1Lock.h>

using namespace BnD;

B1RedisDirectClientSession::B1RedisDirectClientSession(B1ClientSocket* clientSocket, B1BaseClientSessionListener* listener, B1RedisDirectClient* owner)
    : B1ArrayBufferClientSession(clientSocket, listener)
    , _lastReconnectTick(0)
    , _lastAliveCheckTick(0)
    , _aliveCheckCount(0)
    , _owner(owner)
{
}

B1RedisDirectClientSession::~B1RedisDirectClientSession()
{
}

void B1RedisDirectClientSession::implOnRecvRedisSimpleStrings(const B1String& string)
{
    checkAliveString(string);



    //B1LOG("redis_string:[%s]", string.cString());


}

void B1RedisDirectClientSession::implOnRecvRedisErrors(const B1String& message)
{
    B1LOG("redis_error:[%s]", message.cString());
}

void B1RedisDirectClientSession::implOnRecvRedisIntegers(const B1String& int64String)
{
    //B1LOG("redis_int:[%s]", int64String.cString());
}

void B1RedisDirectClientSession::implOnRecvRedisBulkStrings(const B1String& string)
{
    //B1LOG("redis_bulk_string:[%s]", string.cString());
}

void B1RedisDirectClientSession::implOnRecvRedisArrays(const std::list<B1String>& strings)
{
    for (const auto& string : strings) {
        checkAliveString(string);
    }
}

void B1RedisDirectClientSession::onReadComplete(uint8* data, size_t dataSize)
{
    if (analyzeData(data, dataSize) != true) {
        disconnect();
    }
}

void B1RedisDirectClientSession::implOnConnect()
{
    B1LOG("session connected -> clear buffer");
    _aliveCheckCount = 0;
    clearBuffer();
    B1ArrayBufferClientSession::implOnConnect();
}

void B1RedisDirectClientSession::implOnDisconnected(int32 reason)
{
    B1LOG("session disconnected");
    B1ArrayBufferClientSession::implOnDisconnected(reason);
}

void B1RedisDirectClientSession::implProcessConnected(bool firstConnectedProcess)
{
    if (firstConnectedProcess) {
        B1LOG("selecting redis db: db[%d]", owner()->db());
        auto data = packetMaker()->makeDataSelectDB(owner()->db());
        if (writeData(data) != true) {
            disconnect();
            return;
        }
    }
    uint64 now = B1TickUtil::currentTick();
    if (B1TickUtil::diffTick(_lastAliveCheckTick, now) >= owner()->aliveInterval()) {
        _lastAliveCheckTick = now;
        implCheckAlive();
    }
}

void B1RedisDirectClientSession::implProcessDisconnected()
{
    uint64 now = B1TickUtil::currentTick();
    if (B1TickUtil::diffTick(_lastReconnectTick, now) >= B1RedisDirectProtocol::CONSTS_CLIENT_RECONNECT_INTERVAL) {
        _lastReconnectTick = now;
        reconnect();
    }
}

void B1RedisDirectClientSession::implCheckAlive()
{
    auto data = packetMaker()->makeDataPing();
    writeData(data);
    checkAliveCount();
}

B1RedisDirectPacketMaker* B1RedisDirectClientSession::packetMaker()
{
    return owner()->packetMaker();
}

void B1RedisDirectClientSession::checkAliveString(const B1String& string)
{
    if (string.caseInsensitiveCompare("pong")) {
        _aliveCheckCount = 0;
    }
}

void B1RedisDirectClientSession::checkAliveCount()
{
    if (++_aliveCheckCount > owner()->maxAliveCount()) {
        B1LOG("alive check failed -> client force to disconnect: aliveCheckCount[%d], maxAliveCount[%d]", _aliveCheckCount, owner()->maxAliveCount());
        disconnect();
    }
}

bool B1RedisDirectClientSession::isRedisDirectSessionConnected() const
{
    return isConnected() && isFirstConnectedProcessComplete();
}
