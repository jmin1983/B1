//
// B1RedisDirectClientMessageListener.h
//
// Library: B1Redic
// Package: B1Redic
// Module:  B1Redic
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1REDIC_REDIS_DIRECT_CLIENT_MESSAGE_LISTENER_H
#define _B1REDIC_REDIS_DIRECT_CLIENT_MESSAGE_LISTENER_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

namespace BnD {
    class B1String;
    class B1RedisDirectClientMessageListener {
    public:
        virtual void onRecvMessagePublish(const B1String& channel, const B1String& message) {}
        virtual bool onRecvMessagePolling(const B1String& key, const B1String& message) { return true; }    //  return false if message process incompleted.
    };
}   //  !BnD

#endif  // !_B1REDIC_REDIS_DIRECT_CLIENT_MESSAGE_LISTENER_H
