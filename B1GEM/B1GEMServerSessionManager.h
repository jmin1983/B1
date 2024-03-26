//
// B1GEMServerSessionManager.h
//
// Library: B1GEM
// Package: B1GEM
// Module:  B1GEM
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1GEM_SERVER_SESSION_MANAGER_H
#define _B1GEM_SERVER_SESSION_MANAGER_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <B1Base/B1Lock.h>
#include <B1Network/B1BaseSessionManager.h>
#include <B1SECS2/B1SECS2DataTypes.h>

namespace BnD {
    struct B1GEMDataValueCondition;
    class B1GEMServerSessionManager : public B1BaseSessionManager {
    public:
        B1GEMServerSessionManager();
        virtual ~B1GEMServerSessionManager();
    protected:
        B1BaseSocket* _gemSession;
        B1Lock _gemSessionLock; //  only single-session connection allowed.
    public:
        bool setGemSession(void* gemSession);
        bool clearGemSession(void* gemSession);
        bool isFirstSessionCommunicating() const;
        void sendCurrentControlState();
        void sendControlStateOffline();
        void sendControlStateAttemptOnline();
        void sendAlarm(const B1SECS2DataALID& alID, bool isSetAlarm, bool sendRelatedCEID, const B1GEMDataValueCondition& gemCondition);
        void sendEvent(const B1SECS2DataCEID& ceID, const B1GEMDataValueCondition& gemCondition);
    };
}   //  !BnD

#endif  // !_B1GEM_SERVER_SESSION_MANAGER_H
