//
// B1RedisDirectProtocol.h
//
// Library: B1Redic
// Package: B1Redic
// Module:  B1Redic
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1REDIC_REDIS_DIRECT_PROTOCOL_H
#define _B1REDIC_REDIS_DIRECT_PROTOCOL_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

namespace BnD {
    namespace B1RedisDirectProtocol {
        enum CONSTS {
            CONSTS_CLIENT_RECONNECT_INTERVAL = 1000,
        };
        enum RESP_TYPE {
            RESP_TYPE_SIMPLE_STRINGS =  '+',
            RESP_TYPE_ERRORS =          '-',
            RESP_TYPE_INTEGERS =        ':',
            RESP_TYPE_BULK_STRINGS =    '$',
            RESP_TYPE_ARRAYS =          '*',
        };
    };
}   //  !BnD

#endif  // !_B1REDIC_REDIS_DIRECT_PROTOCOL_H
