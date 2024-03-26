//
// B1GEMServer.cpp
//
// Library: B1GEM
// Package: B1GEM
// Module:  B1GEM
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1GEM.h"
#include "B1GEMServer.h"
#include "B1GEMDataRepository.h"
#include "B1GEMServerSession.h"
#include "B1GEMServerSessionManager.h"

using namespace BnD;
using namespace B1GEMConsts;

B1GEMServer::B1GEMServer(uint16 deviceID, B1GEMConsts::COMM_STATE initialCommState, B1GEMConsts::CONTROL_STATE initialControlState)
    : B1SECS2Server(deviceID)
    , _commState(initialCommState)
    , _controlState(initialControlState)
    , _dataRepository()
{
}

B1GEMServer::~B1GEMServer()
{
}

B1BaseSessionManager* B1GEMServer::createSessionManager()
{
    return new B1GEMServerSessionManager();
}

bool B1GEMServer::isAcceptableSession(B1ServerSocket* serverSocket)
{
    if (_commState.isSame(COMM_STATE_DISABLED)) {
        B1LOG("Connected but Communications State is DISABLED: peerAddress[%s], localPort[%d]", peerAddress(serverSocket).cString(), localPort(serverSocket));
        return false;
    }
    return static_cast<B1GEMServerSessionManager*>(_sessionManager.get())->setGemSession(serverSocket);
}

void B1GEMServer::onServerSessionDisconnected(B1ServerSocket* serverSocket, int32 reason)
{
    B1SECS2Server::onServerSessionDisconnected(serverSocket, reason);
    if (static_cast<B1GEMServerSessionManager*>(_sessionManager.get())->clearGemSession(serverSocket)) {
        if (_commState.isSame(COMM_STATE_DISABLED) != true) {
            onCommStateChanged(COMM_STATE_NOT_COMMUNICATING);
        }
    }
}

bool B1GEMServer::implInitialize(int32 port)
{
    if (B1SECS2Server::implInitialize(port) != true) {    //  B1SECS2DataManager 생성 시점 때문에 먼저 B1SECS2Server::implInitialize() 한다.
        return false;
    }
    _dataRepository.reset(createDataRepository());
    if (_dataRepository->initialize(secs2DataManager()) != true) {
        return false;
    }
    return true;
}

void B1GEMServer::implFinalize()
{
    B1SECS2Server::implFinalize();
    if (_dataRepository) {
        _dataRepository->finalize();
        _dataRepository.reset();
    }
}

B1GEMServerSessionManager* B1GEMServer::sessionManager() const
{
    return static_cast<B1GEMServerSessionManager*>(_sessionManager.get());
}

void B1GEMServer::setCommStateEnable(bool flag)
{
    bool disconnectAll = false;
    B1String log;
    {
        B1AutoLock al(_commState._dataLock);
        if (flag) {
            if (COMM_STATE_ENABLED == _commState._data) {
                return;
            }
            _commState._data = COMM_STATE_ENABLED;
            onCommStateChanged(COMM_STATE_NOT_COMMUNICATING);
            log = "Communications State is set to ENABLED";
        }
        else {
            if (COMM_STATE_DISABLED == _commState._data) {
                return;
            }
            _commState._data = COMM_STATE_DISABLED;
            onCommStateChanged(COMM_STATE_DISABLED);
            log = "Communications State is set to DISABLED -> disconnectAll";
            disconnectAll = true;
        }
    }
    if (log.isEmpty() != true) {
        B1LOG(log.cString());
    }
    if (disconnectAll) {
        disconnectAllSessions();
    }
}

void B1GEMServer::setCommStateCommunicating() const
{
    {
        B1AutoLock al(_commState._dataLock);
        if (COMM_STATE_ENABLED != _commState._data) {
            B1LOG("Set Communications State to Communicating. but communications State is not ENABLED");
            assert(false);
            return;
        }
    }
    onCommStateChanged(COMM_STATE_COMMUNICATING);
}

void B1GEMServer::setControlState(CONTROL_STATE state) const
{
    {
        B1AutoLock al(_controlState._dataLock);
        if (_controlState._data == state) {
            return;
        }
        _controlState._data = state;
    }
    switch (state) {
        case CONTROL_STATE_OFFLINE_EQ:
        case CONTROL_STATE_OFFLINE_ATTEMPT_ONLINE:
        case CONTROL_STATE_OFFLINE_HOST:
            sessionManager()->sendControlStateOffline();
            break;
        case CONTROL_STATE_ONLINE_LOCAL:
        case CONTROL_STATE_ONLINE_REMOTE:
            sessionManager()->sendControlStateAttemptOnline();
            break;
        default:
            B1LOG("setControlState UNKNOWN: state[%d]", state);
            return;
    }
    onControlStateChanged(state);
}

void B1GEMServer::getCommState(B1GEMConsts::COMM_STATE* state)
{
    auto commState = _commState.data();
    if (COMM_STATE_DISABLED == commState) {
        *state = COMM_STATE_DISABLED;
    }
    else {
        *state = sessionManager()->isFirstSessionCommunicating() ? COMM_STATE_COMMUNICATING : COMM_STATE_NOT_COMMUNICATING;
    }
}

CONTROL_STATE B1GEMServer::controlState() const
{
    return _controlState.data();
}

void B1GEMServer::sendAlarm(const B1SECS2DataALID& alID, bool isSetAlarm, bool sendRelatedCEID, const B1GEMDataValueCondition& gemCondition)
{
    sessionManager()->sendAlarm(alID, isSetAlarm, sendRelatedCEID, gemCondition);
}

void B1GEMServer::sendEvent(const B1SECS2DataCEID& ceID, const B1GEMDataValueCondition& gemCondition)
{
    sessionManager()->sendEvent(ceID, gemCondition);
}
