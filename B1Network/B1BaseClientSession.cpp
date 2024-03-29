//
// B1BaseClientSession.cpp
//
// Library: B1Network
// Package: B1Network
// Module:  B1Network
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1Network.h"
#include "B1BaseClientSession.h"
#include "B1BaseClientSessionListener.h"
#include "B1ClientSocket.h"
#include "B1NetworkTypeConverter.h"

using namespace BnD;

B1BaseClientSession::B1BaseClientSession(B1ClientSocket* clientSocket, B1BaseClientSessionListener* listener)
    : B1BaseSession(clientSocket)
    , _closeConnection(false)
    , _listener(listener)
{
}

B1BaseClientSession::~B1BaseClientSession()
{
}

void B1BaseClientSession::onConnectComplete()
{
    if (_listener) {
        _listener->onClientSessionConnectResult(clientSocket(), true, 0);
    }
    setSessionStatusConnected();
    if (beginRead() != true) {
        assert(false);
        B1LOG("begin read failed -> disconnect");
        disconnect();
        setSessionStatusDisconnecting();
    }
}

void B1BaseClientSession::onConnectFailed(int32 reason)
{
    if (_listener) {
        _listener->onClientSessionConnectResult(clientSocket(), false, reason);
    }
    setSessionStatusDisconnecting(reason);
}

void B1BaseClientSession::implOnConnectionStatusChanged(STATUS previousStatus, STATUS newStatus)
{
    if (STATUS_DISCONNECTED == newStatus && _closeConnection) {
        if (_listener) {
            _listener->onClientSessionClosed(clientSocket());
        }
    }
}

void B1BaseClientSession::implProcessDisconnected()
{
    if (_closeConnection) {
        if (_listener) {
            _listener->onClientSessionClosed(clientSocket());
        }
    }
    else {
        reconnect();
    }
}

B1ClientSocket* B1BaseClientSession::clientSocket() const
{
    return B1NetworkTypeConverter::toClientSocket(baseSocket());
}

void B1BaseClientSession::reconnect()
{
    setSessionStatusConnecting();
    if (clientSocket()->reconnect() != true) {
        setSessionStatusDisconnecting();
    }
}

bool B1BaseClientSession::connect(B1String&& address, uint16 port)
{
    _closeConnection = false;
    setSessionStatusConnecting();
    if (clientSocket()->connect(std::move(address), port, this) != true) {
        setSessionStatusDisconnecting();
        return false;
    }
    return true;
}

void B1BaseClientSession::closeSession()
{
    _closeConnection = true;
    disconnect();
}
