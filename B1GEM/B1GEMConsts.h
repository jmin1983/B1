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
            COMM_STATE_COMMUNICATING,       //  서버는 이 값으로 설정될 수 없음.
            COMM_STATE_NOT_COMMUNICATING,   //  서버는 이 값으로 설정될 수 없음.

            COMM_STATE_COUNT,
        };
        enum CONTROL_STATE {    //  Control State Model
            CONTROL_STATE_OFFLINE_UNKNOWN = 0,
            CONTROL_STATE_OFFLINE_EQ = 1,
            CONTROL_STATE_OFFLINE_ATTEMPT_ONLINE = 2,   //  S1F1 보낸 이후 상태이므로 세션에서만 의미가 있음. 따라서 서버는 이 값으로 설정될 수 없음.
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
