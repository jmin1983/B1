//
// B1BaseServerSession.cpp
//
// Library: B1Network
// Package: B1Network
// Module:  B1Network
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//
#include "B1Network.h"
#include "B1BaseServerSession.h"
#include "B1BaseServerSessionListener.h"
#include "B1NetworkTypeConverter.h"
#include "B1ServerSocket.h"

using namespace BnD;

B1BaseServerSession::B1BaseServerSession(B1ServerSocket* serverSocket, B1BaseServerSessionListener* listener)
    : B1BaseSession(serverSocket)
    , _listener(listener)
{
}

B1BaseServerSession::~B1BaseServerSession()
{
}

void B1BaseServerSession::implOnDisconnected(int32 reason)
{
    B1LOG("server session disconnected: sessionHandleID[%d], peerAddress[%s], localPort[%d], reason[%d]", _sessionHandleID, 
        serverSocket() ? serverSocket()->peerAddress().cString() : "", serverSocket() ? serverSocket()->localPort() : 0, reason);
    implOnServerSessionDisconnected(reason);
    if (_listener) {
        _listener->onServerSessionDisconnected(serverSocket(), reason);
    }
}

B1ServerSocket* B1BaseServerSession::serverSocket() const
{
    return B1NetworkTypeConverter::toServerSocket(baseSocket());
}

bool B1BaseServerSession::beginRead()
{
    setSessionStatusConnected();
    if (B1BaseSession::beginRead() != true) {
        assert(false);
        B1LOG("begin read failed -> disconnect: peerAddress[%s], localPort:[%d]", baseSocket()->peerAddress().cString(), baseSocket()->localPort());
        disconnect();
        setSessionStatusDisconnecting();
        return false;
    }
    return true;
}
