//
// B1GEMClientSession.cpp
//
// Library: B1GEM
// Package: Client
// Module:  B1GEM
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1GEM.h"
#include "B1GEMClientSession.h"
#include "B1GEMDataRepository.h"

using namespace BnD;
using namespace B1GEMConsts;

B1GEMClientSession::B1GEMClientSession(B1ClientSocket* clientSocket, B1BaseClientSessionListener* listener)
    : B1SECS2ClientSession(clientSocket, listener)
    , _controlState(CONTROL_STATE_OFFLINE_UNKNOWN)
{
}

void B1GEMClientSession::onRecvMessageS1F1(bool wait, const std::vector<uint8>& systemBytes)
{
    sendMessageS1F2(systemBytes);
}

bool B1GEMClientSession::implInitializeSession()
{
    if (B1SECS2ClientSession::implInitializeSession() != true) {
        return false;
    }
    _dataRepository = createDataRepository();
    if (_dataRepository->initialize(secs2DataManager()) != true) {
        return false;
    }
    return true;
}

void B1GEMClientSession::implFinalizeSession()
{
    if (_dataRepository) {
        _dataRepository->finalize();
        _dataRepository.reset();
    }
    B1SECS2ClientSession::implFinalizeSession();
}

void B1GEMClientSession::setControlState(CONTROL_STATE controlState)
{
    if (_controlState == controlState) {
        return;
    }
    B1LOG("control state changed:[%d]->[%d]", _controlState, controlState);
    _controlState = controlState;
}

COMM_STATE B1GEMClientSession::currentCommunicationState() const
{
    return isConnected() ? COMM_STATE_COMMUNICATING : COMM_STATE_NOT_COMMUNICATING;
}
