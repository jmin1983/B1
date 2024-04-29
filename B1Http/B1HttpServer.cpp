//
// B1HttpServer.cpp
//
// Library: B1Http
// Package: Http
// Module:  B1Http
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1Http.h"
#include "B1HttpServer.h"
#include "B1HttpMessage.h"
#include "B1HttpServerSession.h"
#include "B1HttpServerSessionManager.h"
#include "B1WebSocketServerSession.h"

#include <B1Network/B1BaseSessionManager.h>
#include <B1Network/B1NetworkTypeConverter.h>
#include <B1Network/B1ServerSocket.h>

using namespace BnD;

B1HttpServer::B1HttpServer(B1String&& contentsRootPath)
    : B1BaseServer()
    , _contentsRootPath(contentsRootPath)
{
}

B1WebSocketServerSession* B1HttpServer::createWebSocketServerSession(B1ServerSocket* serverSocket)
{
    return new B1WebSocketServerSession(serverSocket, this);
}

void B1HttpServer::onHttpServerSessionUpgradeRequested(B1ServerSocket* serverSocket, const B1HttpMessage& httpMessage)
{
    auto baseSocket = B1NetworkTypeConverter::toBaseSocket(serverSocket);
    auto baseSession = sessionManager()->getBaseSession(baseSocket);

    auto httpMessageMoved = std::move(httpMessage);
    baseSession->finalizeSession();

    std::shared_ptr<B1WebSocketServerSession> session(createWebSocketServerSession(serverSocket));
    if (session->initializeSession() != true) {
        B1LOG("initialize session failed -> disconnect: peerAddress[%s], localPort:[%d]", serverSocket->peerAddress().cString(), serverSocket->localPort());
        disconnect(serverSocket);
        assert(false);
        return;
    }
    session->acceptWebSocket(httpMessageMoved);
    if (sessionManager()->modifySession(B1NetworkTypeConverter::toBaseSocket(serverSocket), session) != true) {
        B1LOG("modify session failed -> disconnect: peerAddress[%s], localPort:[%d]", serverSocket->peerAddress().cString(), serverSocket->localPort());
        disconnect(serverSocket);
        assert(false);
    }
}

B1BaseSessionManager* B1HttpServer::createSessionManager()
{
    return new B1HttpServerSessionManager();
}

B1BaseServerSession* B1HttpServer::createSession(B1ServerSocket* serverSocket)
{
    return new B1HttpServerSession(serverSocket, this, this, _contentsRootPath);
}

bool B1HttpServer::implInitialize(int32 port)
{
    if (port < 1 || port - 1 > UINT16_MAX) {
        return false;
    }
    if (startup() != true) {
        return false;
    }
    if (accept(static_cast<uint16>(port)) != true) {
        return false;
    }
    return true;
}

void B1HttpServer::implFinalize()
{
    close();
    shutdown();
}

std::map<int32, std::shared_ptr<B1BaseSession> > B1HttpServer::allWebSocketSessions() const
{
    return static_cast<B1HttpServerSessionManager*>(sessionManager())->webSocketSessions();
}

bool B1HttpServer::initialize(int32 port)
{
    return implInitialize(port);
}

void B1HttpServer::finalize()
{
    implFinalize();
}
