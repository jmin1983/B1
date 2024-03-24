//
// B1RedisDirectClientPollingSession.h
//
// Library: B1Redic
// Package: B1Redic
// Module:  B1Redic
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1REDIC_REDIS_DIRECT_CLIENT_POLLING_SESSION_H
#define _B1REDIC_REDIS_DIRECT_CLIENT_POLLING_SESSION_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif
 
#include <B1Redic/B1RedisDirectClientReadSession.h>

namespace BnD {
    class B1RedisDirectClientPollingSession : public B1RedisDirectClientReadSession {
    public:
        B1RedisDirectClientPollingSession(B1ClientSocket* clientSocket, B1BaseClientSessionListener* listener, B1RedisDirectClient* owner);
        virtual ~B1RedisDirectClientPollingSession();
    protected:
        bool _startPolling;
    protected:
        void implProcessConnected(bool firstConnectedProcess) final;
    protected:
        bool lindex(const B1String& key, int32 index, B1String* out);
        bool lpop(const B1String& key);
    public:
        void startPolling() { _startPolling = true; }
    };
}   //  !BnD

#endif  // !_B1REDIC_REDIS_DIRECT_CLIENT_POLLING_SESSION_H
