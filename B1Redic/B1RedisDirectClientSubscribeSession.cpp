//
// B1RedisDirectClientSubscribeSession.cpp
//
// Library: B1Redic
// Package: B1Redic
// Module:  B1Redic
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1Redic.h"
#include "B1RedisDirectClientSubscribeSession.h"
#include "B1RedisDirectClient.h"
#include "B1RedisDirectPacketMaker.h"

using namespace BnD;

B1RedisDirectClientSubscribeSession::B1RedisDirectClientSubscribeSession(B1ClientSocket* clientSocket, B1BaseClientSessionListener* listener, B1RedisDirectClient* owner)
    : B1RedisDirectClientSession(clientSocket, listener, owner)
    , _subscribeChannelsLock()
    , _subscribeChannels()
{
}

B1RedisDirectClientSubscribeSession::~B1RedisDirectClientSubscribeSession()
{
}

void B1RedisDirectClientSubscribeSession::implOnRecvRedisArrays(const std::list<B1String>& strings)
{
    if (strings.size() == 3 && strings.front().caseInsensitiveCompare("message")) {
        auto itr = strings.begin();
        const B1String& channel = *(++itr);
        const B1String& message = *(++itr);
        owner()->onRecvMessagePublish(channel, message);
    }
    else {
        B1RedisDirectClientSession::implOnRecvRedisArrays(strings);
    }
}

void B1RedisDirectClientSubscribeSession::implProcessConnected(bool firstConnectedProcess)
{
    B1RedisDirectClientSession::implProcessConnected(firstConnectedProcess);
    if (firstConnectedProcess) {
        std::vector<uint8> data;
        {
            B1AutoLock al(_subscribeChannelsLock);
            if (_subscribeChannels.empty() != true) {
                data = packetMaker()->makeDataSubscribe(_subscribeChannels);
            }
        }
        if (data.empty() != true) {
            if (writeData(data) != true) {
                disconnect();
                return;
            }
        }
    }
}

bool B1RedisDirectClientSubscribeSession::subscribe(std::set<B1String>&& channels)
{
    std::vector<uint8> data;
    {
        B1AutoLock al(_subscribeChannelsLock);
        _subscribeChannels.swap(channels);
        data = packetMaker()->makeDataSubscribe(_subscribeChannels);
    }
    return writeData(data);
}
