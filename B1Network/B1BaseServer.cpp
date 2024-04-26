//
// B1BaseServer.cpp
//
// Library: B1Network
// Package: B1Network
// Module:  B1Network
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1Network.h"
#include "B1BaseServer.h"
#include "B1Acceptor.h"
#include "B1BaseServerSession.h"
#include "B1BaseSessionManager.h"
#include "B1IOLooper.h"
#include "B1ServerSocket.h"

using namespace BnD;

B1BaseServer::B1BaseServer()
{
}

B1BaseServer::~B1BaseServer()
{
}

void B1BaseServer::onAcceptComplete(std::shared_ptr<B1ServerSocket> serverSocket)
{
    auto session = std::shared_ptr<B1BaseServerSession>(createSession(serverSocket.get()));
    if (session->initialize() != true) {
        B1LOG("initialize session failed -> disconnect: peerAddress[%s], localPort:[%d]", serverSocket->peerAddress().cString(), serverSocket->localPort());
        disconnect(serverSocket.get());
        assert(false);
        return;
    }
    int32 handleID = sessionManager()->addSession(serverSocket, session);
    session->setSessionHandleID(handleID);
    if (isAcceptableSession(serverSocket.get()) != true) {
        B1LOG("not acceptable session -> disconnect: peerAddress[%s], localPort:[%d]", serverSocket->peerAddress().cString(), serverSocket->localPort());
        disconnect(serverSocket.get());
        sessionManager()->reserveRemoveSession(serverSocket.get());
        return;
    }
    if (session->beginRead() != true) {
        B1LOG("unable to beginRead -> disconnect: peerAddress[%s], localPort:[%d]", serverSocket->peerAddress().cString(), serverSocket->localPort());
        disconnect(serverSocket.get());
        sessionManager()->reserveRemoveSession(serverSocket.get());
        return;
    }
}

void B1BaseServer::onAcceptFailed(std::shared_ptr<B1ServerSocket> serverSocket, int32 reason)
{
    B1LOG("accept error: peerAddress[%s], localPort[%d], reason[%d]", serverSocket->peerAddress().cString(), serverSocket->localPort(), reason);
}

void B1BaseServer::onServerSessionDisconnected(B1ServerSocket* serverSocket, int32 reason)
{
    _sessionManager->reserveRemoveSession(serverSocket);
}

bool B1BaseServer::implStartup()
{
    _ioLooper.reset(new B1IOLooper());
    return _ioLooper->startup();
}

void B1BaseServer::implShutdown()
{
    if (_ioLooper) {
        _ioLooper->shutdown();
        _ioLooper.reset();
    }
}

bool B1BaseServer::accept(uint16 port)
{
    B1LOG("begin accept: port[%d]", port);
    _acceptor.reset(new B1Acceptor(_ioLooper->ioContext()));
    if (_acceptor->initialize(port, this) != true) {
        _acceptor.reset();
        return false;
    }
    _ioLooper->pause();
    _ioLooper->resume();
    return true;
}

void B1BaseServer::close()
{
    if (_acceptor) {
        _acceptor->finalize();
        _acceptor.reset();
    }
}

void B1BaseServer::disconnect(B1ServerSocket* serverSocket)
{
    serverSocket->close();
}

void B1BaseServer::disconnectAllSessions()
{
    _sessionManager->disconnectAllSessions();
}

B1String B1BaseServer::peerAddress(B1ServerSocket* serverSocket) const
{
    return serverSocket->peerAddress();
}

uint16 B1BaseServer::localPort(B1ServerSocket* serverSocket) const
{
    return serverSocket->localPort();
}
