//
// B1RedisDirectClientSession.h
//
// Library: B1Redic
// Package: B1Redic
// Module:  B1Redic
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1REDIC_REDIS_DIRECT_CLIENT_SESSION_H
#define _B1REDIC_REDIS_DIRECT_CLIENT_SESSION_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif
 
#include <B1Network/B1ArrayBufferClientSession.h>
#include <B1Redic/B1RedisDirectPacketAnalyzer.h>

namespace BnD {
    class B1RedisDirectClient;
    class B1RedisDirectPacketMaker;
    class B1RedisDirectClientSession : protected B1RedisDirectPacketAnalyzer
                                     , public B1ArrayBufferClientSession {
    public:
        B1RedisDirectClientSession(B1ClientSocket* clientSocket, B1BaseClientSessionListener* listener, B1RedisDirectClient* owner);
        virtual ~B1RedisDirectClientSession();
    private:
        uint64 _lastReconnectTick;
        uint64 _lastAliveCheckTick;
    protected:
        int32 _aliveCheckCount;
        B1RedisDirectClient* _owner;
    protected:  //  B1RedisDirectPacketAnalyzer
        virtual void implOnRecvRedisSimpleStrings(const B1String& string) override;
        virtual void implOnRecvRedisErrors(const B1String& message) override;
        virtual void implOnRecvRedisIntegers(const B1String& int64String) override;
        virtual void implOnRecvRedisBulkStrings(const B1String& string) override;
        virtual void implOnRecvRedisArrays(const std::list<B1String>& strings) override;
    protected:  //  B1ArrayBufferClientSession
        void onReadComplete(uint8* data, size_t dataSize) final;
        virtual void implOnConnect() override;
        virtual void implOnDisconnected(int32 reason) override;
        virtual void implProcessConnected(bool firstConnectedProcess) override;
        void implProcessDisconnected() final;
    protected:
        virtual void implCheckAlive();
    protected:
        B1RedisDirectClient* owner() { return _owner; }
        B1RedisDirectPacketMaker* packetMaker();
        void checkAliveString(const B1String& string);
        void checkAliveCount();
    public:
        bool isRedisDirectSessionConnected() const;
    };
}   //  !BnD

#endif  // !_B1REDIC_REDIS_DIRECT_CLIENT_SESSION_H
