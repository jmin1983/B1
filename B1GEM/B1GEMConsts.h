//
// B1GEMConsts.h
//
// Library: B1GEM
// Package: B1GEM
// Module:  B1GEM
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1GEM_CONSTS_H
#define _B1GEM_CONSTS_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

namespace BnD {
    namespace B1GEMConsts {
        enum COMM_STATE {   //  Communications State Model
            COMM_STATE_DISABLED = 0,
            COMM_STATE_ENABLED,
            COMM_STATE_COMMUNICATING,       //  server can not be set to this value.
            COMM_STATE_NOT_COMMUNICATING,   //  server can not be set to this value.

            COMM_STATE_COUNT,
        };
        enum CONTROL_STATE {    //  Control State Model
            CONTROL_STATE_OFFLINE_UNKNOWN = 0,
            CONTROL_STATE_OFFLINE_EQ = 1,
            CONTROL_STATE_OFFLINE_ATTEMPT_ONLINE = 2,   //  server can not be set to this value.
            CONTROL_STATE_OFFLINE_HOST = 3,
            CONTROL_STATE_ONLINE_LOCAL = 4,
            CONTROL_STATE_ONLINE_REMOTE = 5,

            CONTROL_STATE_COUNT,

            CONTROL_STATE_MIN = CONTROL_STATE_OFFLINE_UNKNOWN,
            CONTROL_STATE_MAX = CONTROL_STATE_ONLINE_REMOTE,
        };
    };
}   //  !BnD

#endif  // !_B1GEM_CONSTS_H
