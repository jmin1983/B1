//
// B1RedisDirectClientSubscribeSession.h
//
// Library: B1Redic
// Package: B1Redic
// Module:  B1Redic
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1REDIC_REDIS_DIRECT_CLIENT_SUBSCRIBE_SESSION_H
#define _B1REDIC_REDIS_DIRECT_CLIENT_SUBSCRIBE_SESSION_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif
 
#include <B1Redic/B1RedisDirectClientSession.h>

#include <set>

namespace BnD {
    class B1RedisDirectClientSubscribeSession : public B1RedisDirectClientSession {
    public:
        B1RedisDirectClientSubscribeSession(B1ClientSocket* clientSocket, B1BaseClientSessionListener* listener, B1RedisDirectClient* owner);
        virtual ~B1RedisDirectClientSubscribeSession();
    protected:
        std::set<B1String> _subscribeChannels;
    protected:  //  B1RedisDirectClientSession
        void implOnRecvRedisArrays(const std::list<B1String>& strings) final;
    protected:
        void implProcessConnected(bool firstConnectedProcess) final;
    public:
        bool subscribe(std::set<B1String>&& channels);
    };
}   //  !BnD

#endif  // !_B1REDIC_REDIS_DIRECT_CLIENT_SUBSCRIBE_SESSION_H
