//
// B1BaseSession.cpp
//
// Library: B1Network
// Package: B1Network
// Module:  B1Network
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1Network.h"
#include "B1BaseSession.h"
#include "B1BaseSocket.h"
#include "B1BaseSocketImpl.h"
#include "B1BaseReadWriteImpl.h"

using namespace BnD;

B1BaseSession::B1BaseSession(B1BaseSocket* baseSocket)
    : _firstConnectedProcess(false)
    , _baseSocket(baseSocket)   //  can be NULL.
    , _disconnectingReason(0)
    , _lastConnectionStatus(STATUS_NONE)
    , _connectionStatus(STATUS_NONE)
    , _sessionHandleID(-1)
    , _readWriteImpl()
{
}

B1BaseSession::~B1BaseSession()
{
}

B1BaseSession::B1BaseSession(const B1BaseSession& baseSession, bool firstConnectedProcess)
    : _firstConnectedProcess(firstConnectedProcess)
    , _baseSocket(baseSession._baseSocket)
    , _connectionStatus(baseSession._connectionStatus)
    , _sessionHandleID(-1)
    , _readWriteImpl()
{
}

void B1BaseSession::setSessionStatusDisconnected()
{
    const auto previousStatus = _connectionStatus;
    const auto newStatus = STATUS_DISCONNECTED;

    B1LOG("set connection_status to STATUS_DISCONNECTED: sessionHandleID[%d], previous_status:[%d], reason[%d]", _sessionHandleID, previousStatus, _disconnectingReason);
    _connectionStatus = newStatus;
    if (previousStatus != newStatus) {
        implOnConnectionStatusChanged(previousStatus, newStatus);
        if (STATUS_CONNECTED == _lastConnectionStatus) {
            implOnDisconnected(_disconnectingReason);
        }
        _lastConnectionStatus = newStatus;
    }
}

void B1BaseSession::implDisconnect()
{
    if (_readWriteImpl) {
        _readWriteImpl->close();
    }
}

void B1BaseSession::setSessionStatusConnecting()
{
    const auto previousStatus = _connectionStatus;
    const auto newStatus = STATUS_CONNECTING;

    B1LOG("set connection_status to STATUS_CONNECTING: sessionHandleID[%d], previous_status:[%d]", _sessionHandleID, previousStatus);
    _connectionStatus = newStatus;
    if (previousStatus != newStatus) {
        implOnConnectionStatusChanged(previousStatus, newStatus);
    }
}

void B1BaseSession::setSessionStatusConnected()
{
    implOnConnect();
    _firstConnectedProcess = true;    
    const auto previousStatus = _connectionStatus;
    const auto newStatus = STATUS_CONNECTED;

    B1LOG("set connection_status to STATUS_CONNECTED: sessionHandleID[%d], previous_status:[%d]", _sessionHandleID, previousStatus);
    _connectionStatus = newStatus;
    if (previousStatus != newStatus) {
        implOnConnectionStatusChanged(previousStatus, newStatus);
    }
}

void B1BaseSession::setSessionStatusDisconnecting(int32 reason)
{
    const auto previousStatus = _connectionStatus;
    const auto newStatus = STATUS_DISCONNECTING;

    B1LOG("set connection_status to STATUS_DISCONNECTING: sessionHandleID[%d], previous_status:[%d], reason[%d]", _sessionHandleID, previousStatus, reason);
    _lastConnectionStatus = previousStatus;
    _disconnectingReason = reason;
    _connectionStatus = newStatus;
    if (previousStatus != newStatus) {
        implOnConnectionStatusChanged(previousStatus, newStatus);
    }
}

bool B1BaseSession::beginRead()
{
    return _readWriteImpl ? _readWriteImpl->read() : false;
}

bool B1BaseSession::initialize()
{
    if (_readWriteImpl != NULL) {
        return false;
    }
    _readWriteImpl.reset(createReadWriteImpl());
    if (_readWriteImpl->initialize() != true) {
        return false;
    }
    _baseSocket->setImpl(_readWriteImpl->baseSocketImpl());
    return true;
}

void B1BaseSession::finalize()
{
    if (_readWriteImpl) {
        _baseSocket->resetImpl();
        _readWriteImpl->finalize();
        _readWriteImpl.reset();
    }
}

bool B1BaseSession::isDisconnecting() const
{
    return STATUS_DISCONNECTING == _connectionStatus;
}

bool B1BaseSession::isDisconnected() const
{
    return STATUS_DISCONNECTED == _connectionStatus;
}

bool B1BaseSession::isConnecting() const
{
    return STATUS_CONNECTING == _connectionStatus;
}

bool B1BaseSession::isConnected() const
{
    return STATUS_CONNECTED == _connectionStatus && _readWriteImpl && _readWriteImpl->isOpen();
}

void B1BaseSession::disconnect()
{
    B1LOG("disconnect called: sessionHandleID[%d]", _sessionHandleID);
    implDisconnect();
}

void B1BaseSession::processConnected()
{
    implProcessConnected(_firstConnectedProcess);
    _firstConnectedProcess = false;
}

void B1BaseSession::processDisconnected()
{
    implProcessDisconnected();
}

B1String B1BaseSession::peerAddress() const
{
    return _readWriteImpl ? _readWriteImpl->peerAddress() : "";
}

uint16 B1BaseSession::peerPort() const
{
    return _readWriteImpl ? _readWriteImpl->peerPort() : 0;
}

uint16 B1BaseSession::localPort() const
{
    return _readWriteImpl ? _readWriteImpl->localPort() : 0;
}

int32 B1BaseSession::currentConnectionStatus() const
{
    return _connectionStatus;
}

void B1BaseSession::setDisconnectedIfSessionClosed()
{
    if (isDisconnecting()) {
        if (_readWriteImpl && _readWriteImpl->isClosed()) {
            setSessionStatusDisconnected();
        }
    }
}
