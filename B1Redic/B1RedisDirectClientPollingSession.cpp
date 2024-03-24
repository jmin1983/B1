//
// B1RedisDirectClientPollingSession.cpp
//
// Library: B1Redic
// Package: B1Redic
// Module:  B1Redic
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1Redic.h"
#include "B1RedisDirectClientPollingSession.h"
#include "B1RedisDirectClient.h"
#include "B1RedisDirectClientMessageListener.h"
#include "B1RedisDirectPacketMaker.h"

#include <B1Base/B1Lock.h>

using namespace BnD;

B1RedisDirectClientPollingSession::B1RedisDirectClientPollingSession(B1ClientSocket* clientSocket, B1BaseClientSessionListener* listener, B1RedisDirectClient* owner)
    : B1RedisDirectClientReadSession(clientSocket, listener, owner)
    , _startPolling(false)
{
}

B1RedisDirectClientPollingSession::~B1RedisDirectClientPollingSession()
{
}

void B1RedisDirectClientPollingSession::implProcessConnected(bool firstConnectedProcess)
{
    if (firstConnectedProcess || _startPolling != true) {
        B1RedisDirectClientReadSession::implProcessConnected(firstConnectedProcess);
        return;
    }
    while (true) {
        B1String message;
        if (lindex(owner()->pollingKey(), 0, &message) != true) {
            disconnect();
            return;
        }
        if (message.isEmpty()) {
            break;
        }
        if (owner()->messageListener()->onRecvMessagePolling(owner()->pollingKey(), message) != true) {
            break;
        }
        if (lpop(owner()->pollingKey()) != true) {
            disconnect();
            return;
        }
    }
}

bool B1RedisDirectClientPollingSession::lindex(const B1String& key, int32 index, B1String* out)
{
    _lock->lock();
    if (setWaitData() != true) {
        _lock->unlock();
        return false;
    }
    {
        auto data = packetMaker()->makeDataLindex(key, index);
        if (writeData(data) != true) {
            _lock->unlock();
            return false;
        }
    }
    {
        std::list<B1String> data;
        if (waitData(&data) != true) {
            _lock->unlock();
            return false;
        }
        _lock->unlock();
        if (data.empty() != true) {
            *out = std::move(data.front());
        }
    }
    return true;
}

bool B1RedisDirectClientPollingSession::lpop(const B1String& key)
{
    _lock->lock();
    if (setWaitData() != true) {
        _lock->unlock();
        return false;
    }
    {
        auto data = packetMaker()->makeDataLpop(key);
        if (writeData(data) != true) {
            _lock->unlock();
            return false;
        }
    }
    {
        std::list<B1String> data;
        if (waitData(&data) != true) {
            _lock->unlock();
            return false;
        }
    }
    _lock->unlock();
    //  should check "OK" ?
    return true;
}
