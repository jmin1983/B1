//
// B1RedisDirectClientWriteSession.cpp
//
// Library: B1Redic
// Package: B1Redic
// Module:  B1Redic
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1Redic.h"
#include "B1RedisDirectClientWriteSession.h"
#include "B1RedisDirectClient.h"
#include "B1RedisDirectClientListener.h"
#include "B1RedisDirectPacketMaker.h"

#include <B1Base/B1Lock.h>
#include <B1Base/B1Thread.h>

using namespace BnD;

B1RedisDirectClientWriteSession::B1RedisDirectClientWriteSession(B1ClientSocket* clientSocket, B1BaseClientSessionListener* listener, B1RedisDirectClient* owner, bool startWriting)
    : B1RedisDirectClientSession(clientSocket, listener, owner)
    , _essentialPacketLock(new B1Lock())
    , _essentialPackets()
    , _startWriting(startWriting)
{
}

B1RedisDirectClientWriteSession::~B1RedisDirectClientWriteSession()
{
}

void B1RedisDirectClientWriteSession::implOnDisconnected(int32 reason)
{
    if (auto listener = owner()->listener()) {
        listener->onRedisDirectWriterDisconnected();
    }
    B1RedisDirectClientSession::implOnDisconnected(reason);
}

void B1RedisDirectClientWriteSession::implProcessConnected(bool firstConnectedProcess)
{
    B1RedisDirectClientSession::implProcessConnected(firstConnectedProcess);
    if (firstConnectedProcess) {
        if (_startWriting) {
            sendEssentialPackets();
        }
        if (auto listener = owner()->listener()) {
            listener->onRedisDirectWriterConnected();
        }
    }
}

void B1RedisDirectClientWriteSession::sendEssentialPackets()
{
    B1AutoLock al(*_essentialPacketLock);
    for (const auto& essentialPacket : _essentialPackets) {
        if (writeData(essentialPacket) != true) {
            std::vector<char> buffer(essentialPacket.begin(), essentialPacket.end());
            buffer.push_back(0);
            B1LOG("sendData failed:[%s]", &buffer[0]);
        }
    }
    _essentialPackets.clear();
}

bool B1RedisDirectClientWriteSession::isWritingAvailable() const
{
    return _startWriting && isConnected();
}

bool B1RedisDirectClientWriteSession::trySendData(const std::vector<uint8>& data, bool isEssential)
{
    if (isWritingAvailable()) {
        return writeData(data);
    }
    else {
        if (isEssential) {
            B1AutoLock al(*_essentialPacketLock);
            if (_essentialPackets.size() > CONSTS_MAX_ESSENTIAL_PACKET_COUNT) {
                _essentialPackets.pop_front();
            }
            _essentialPackets.push_back(data);
        }
        return false;
    }
}

void B1RedisDirectClientWriteSession::startWriting()
{
    if (_startWriting) {
        return;
    }
    _startWriting = true;
    sendEssentialPackets();
    B1Thread::sleep(10); //  타이밍 이슈 방지를 위해.
    sendEssentialPackets();
}

bool B1RedisDirectClientWriteSession::publish(const B1String& channel, const B1String& message, bool isEssential)
{
    return trySendData(packetMaker()->makeDataPublish(channel, message), isEssential);
}

bool B1RedisDirectClientWriteSession::hmset(const B1String& key, const std::map<B1String, B1String>& fieldValues, bool isEssential)
{
    return trySendData(packetMaker()->makeDataHmset(key, fieldValues), isEssential);
}

bool B1RedisDirectClientWriteSession::nonBlockedCommand(const std::vector<B1String>& args, bool isEssential)
{
    return trySendData(packetMaker()->makeDataArgs(args), isEssential);
}

bool B1RedisDirectClientWriteSession::hdel(const B1String& key, const B1String& field)
{
    return trySendData(packetMaker()->makeDataHdel(key, field), true);
}

bool B1RedisDirectClientWriteSession::del(const B1String& key)
{
    return trySendData(packetMaker()->makeDataDel(key), true);
}

bool B1RedisDirectClientWriteSession::sadd(const B1String& key, const B1String& value)
{
    return trySendData(packetMaker()->makeDataSadd(key, value), true);
}

bool B1RedisDirectClientWriteSession::srem(const B1String& key, const B1String& value)
{
    return trySendData(packetMaker()->makeDataSrem(key, value), true);
}

bool B1RedisDirectClientWriteSession::rpush(const B1String& key, const B1String& value)
{
    return trySendData(packetMaker()->makeDataRpush(key, value), false);
}
