//
// B1RedisDirectClientListener.h
//
// Library: B1Redic
// Package: B1Redic
// Module:  B1Redic
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1REDIC_REDIS_DIRECT_CLIENT_LISTENER_H
#define _B1REDIC_REDIS_DIRECT_CLIENT_LISTENER_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

namespace BnD {
    class B1String;
    class B1RedisDirectClientListener {
    public:
        virtual void onRedisDirectReaderConnected() {}
        virtual void onRedisDirectReaderDisconnected() {}
        virtual void onRedisDirectWriterConnected() {}
        virtual void onRedisDirectWriterDisconnected() {}
    };
}   //  !BnD

#endif  // !_B1REDIC_REDIS_DIRECT_CLIENT_LISTENER_H
