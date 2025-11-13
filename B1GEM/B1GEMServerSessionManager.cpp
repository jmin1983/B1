//
// B1GEMServerSessionManager.cpp
//
// Library: B1GEM
// Package: Server
// Module:  B1GEM
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1GEM.h"
#include "B1GEMServerSessionManager.h"
#include "B1GEMServerSession.h"

using namespace BnD;

B1GEMServerSessionManager::B1GEMServerSessionManager()
    : B1BaseSessionManager()
    , _gemSession(NULL)
    , _gemSessionLock()
{
}

B1GEMServerSessionManager::~B1GEMServerSessionManager()
{
}

bool B1GEMServerSessionManager::setGemSession(void* gemSession)
{
    B1AutoLock al(_gemSessionLock);
    if (_gemSession != NULL) {
        return false;
    }
    _gemSession = (B1BaseSocket*)gemSession;
    return true;
}

bool B1GEMServerSessionManager::clearGemSession(void* gemSession)
{
    B1AutoLock al(_gemSessionLock);
    if (_gemSession != gemSession) {
        return false;
    }
    _gemSession = NULL;
    return true;
}

bool B1GEMServerSessionManager::isFirstSessionCommunicating() const
{
    auto session = getSession<B1GEMServerSession>(_gemSession);
    return session ? session->isCommunicating() : false;
}

void B1GEMServerSessionManager::sendCurrentControlState()
{
    if (auto session = getSession<B1GEMServerSession>(_gemSession)) {
        session->sendCurrentControlState();
    }
}

void B1GEMServerSessionManager::sendControlStateOffline()
{
    if (auto session = getSession<B1GEMServerSession>(_gemSession)) {
        session->sendControlStateOffline();
    }
}

void B1GEMServerSessionManager::sendControlStateAttemptOnline()
{
    if (auto session = getSession<B1GEMServerSession>(_gemSession)) {
        session->sendControlStateAttemptOnline();
    }
}

void B1GEMServerSessionManager::sendAlarm(const B1SECS2DataALID& alID, bool isSetAlarm, bool sendRelatedCEID, const B1GEMDataValueCondition& gemCondition)
{
    if (auto session = getSession<B1GEMServerSession>(_gemSession)) {
        session->sendAlarm(alID, isSetAlarm, sendRelatedCEID, gemCondition);
    }
}

void B1GEMServerSessionManager::sendEvent(const B1SECS2DataCEID& ceID, const B1GEMDataValueCondition& gemCondition)
{
    if (auto session = getSession<B1GEMServerSession>(_gemSession)) {
        session->sendEvent(ceID, gemCondition);
    }
}
