//
// B1GEMClient.cpp
//
// Library: B1GEM
// Package: Client
// Module:  B1GEM
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1GEM.h"
#include "B1GEMClient.h"
#include "B1GEMClientSession.h"

#include <B1Network/B1BaseSessionManager.h>

using namespace BnD;
using namespace B1GEMConsts;

B1GEMClient::B1GEMClient()
{
}

B1GEMClient::~B1GEMClient()
{
}

std::shared_ptr<B1GEMClientSession> B1GEMClient::getSession(uint16 deviceID) const
{
    return sessionManager()->getSessionByHandleID<B1GEMClientSession>(deviceID);
}

COMM_STATE B1GEMClient::currentCommunicationState(uint16 deviceID) const
{
    if (auto session = getSession(deviceID)) {
        return session->currentCommunicationState();
    }
    return COMM_STATE_NOT_COMMUNICATING;
}