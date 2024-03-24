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
    , _baseSocket(baseSocket)
    , _connectionStatus(STATUS_NONE)
    , _sessionHandleID(-1)
    , _baseSocketImpl()
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
    , _baseSocketImpl()
    , _readWriteImpl()
{
}

B1BaseSocketImpl* B1BaseSession::createBaseSocketImpl()
{
    return new B1ASIOSocketImpl();
}

void B1BaseSession::implDisconnect()
{
    baseSocket()->close();
}

void B1BaseSession::setSessionStatusConnecting()
{
    const auto previousStatus = _connectionStatus;
    const auto newStatus = STATUS_CONNECTING;

    B1LOG("set connection_status to STATUS_CONNECTING: previous_status:[%d]", previousStatus);
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

    B1LOG("set connection_status to STATUS_CONNECTED: previous_status:[%d]", previousStatus);
    _connectionStatus = newStatus;
    if (previousStatus != newStatus) {
        implOnConnectionStatusChanged(previousStatus, newStatus);
    }
}

void B1BaseSession::setSessionStatusDisconnected(int32 reason)
{
    const auto previousStatus = _connectionStatus;
    const auto newStatus = STATUS_DISCONNECTED;

    B1LOG("set connection_status to STATUS_DISCONNECTED: previous_status:[%d], reason[%d]", previousStatus, reason);
    _connectionStatus = newStatus;
    if (previousStatus != newStatus) {
        implOnConnectionStatusChanged(previousStatus, newStatus);
        if (STATUS_CONNECTED == previousStatus) {
            implOnDisconnected(reason);
        }
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
    _baseSocketImpl.reset(createBaseSocketImpl());
    baseSocket()->setImpl(_baseSocketImpl.get());
    _readWriteImpl.reset(createReadWriteImpl(baseSocket()));
    return true;
}

void B1BaseSession::finalize()
{
    if (_readWriteImpl) {
        _readWriteImpl.reset();
    }
    if (_baseSocketImpl) {
        _baseSocketImpl.reset();
    }
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
    return STATUS_CONNECTED == _connectionStatus && baseSocket() && baseSocket()->isOpen();
}

void B1BaseSession::disconnect()
{
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
    return baseSocket()->peerAddress();
}

uint16 B1BaseSession::peerPort() const
{
    return baseSocket()->peerPort();
}

uint16 B1BaseSession::localPort() const
{
    return baseSocket()->localPort();
}

int32 B1BaseSession::currentConnectionStatus() const
{
    return _connectionStatus;
}
