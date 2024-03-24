//
// B1RedisDirectClientWriteSession.h
//
// Library: B1Redic
// Package: B1Redic
// Module:  B1Redic
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1REDIC_REDIS_DIRECT_CLIENT_WRITE_SESSION_H
#define _B1REDIC_REDIS_DIRECT_CLIENT_WRITE_SESSION_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif
 
#include <B1Redic/B1RedisDirectClientSession.h>

#include <map>

namespace BnD {
    class B1Lock;
    class B1RedisDirectClientWriteSession : public B1RedisDirectClientSession {
    public:
        B1RedisDirectClientWriteSession(B1ClientSocket* clientSocket, B1BaseClientSessionListener* listener, B1RedisDirectClient* owner, bool startWriting);
        virtual ~B1RedisDirectClientWriteSession();
    private:
        enum CONSTS {
            CONSTS_MAX_ESSENTIAL_PACKET_COUNT = 1000,
        };
        std::shared_ptr<B1Lock> _essentialPacketLock;
        std::list<std::vector<uint8> > _essentialPackets;
    protected:
        bool _startWriting;
    protected:
        void implOnDisconnected(int32 reason) final;
        void implProcessConnected(bool firstConnectedProcess) final;
    protected:
        void sendEssentialPackets();
        bool isWritingAvailable() const;
        bool trySendData(const std::vector<uint8>& data, bool isEssential);
    public:
        void startWriting();
        bool publish(const B1String& channel, const B1String& message, bool isEssential);
        bool hmset(const B1String& key, const std::map<B1String, B1String>& fieldValues, bool isEssential);
        bool nonBlockedCommand(const std::vector<B1String>& args, bool isEssential);
        bool hdel(const B1String& key, const B1String& field);  //  Essential only.
        bool del(const B1String& key);                          //  Essential only.
        bool sadd(const B1String& key, const B1String& value);  //  Essential only.
        bool srem(const B1String& key, const B1String& value);  //  Essential only.
        bool rpush(const B1String& key, const B1String& value); //  Non-Essential only.
    };
}   //  !BnD

#endif  // !_B1REDIC_REDIS_DIRECT_CLIENT_WRITE_SESSION_H
