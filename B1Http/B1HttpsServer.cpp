//
// B1HttpsServer.cpp
//
// Library: B1Http
// Package: Https
// Module:  B1Http
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1Http.h"
#include "B1HttpsServer.h"
#include "B1HttpsServerSession.h"
#include "B1SecureWebSocketServerSession.h"

#include <B1Network/B1SSLContext.h>

using namespace BnD;

B1HttpsServer::B1HttpsServer(B1String&& contentsRootPath)
    : B1HttpServer(std::move(contentsRootPath))
    , _sslContext(new B1SSLContext(true))
{
}

B1WebSocketServerSession* B1HttpsServer::createWebSocketServerSession(B1ServerSocket* serverSocket)
{
    return new B1SecureWebSocketServerSession(serverSocket, this);
}

B1BaseServerSession* B1HttpsServer::createSession(B1ServerSocket* serverSocket)
{
    //  TODO: consider using beast::async_detect_ssl for plain session.
    return new B1HttpsServerSession(serverSocket, this, this, _contentsRootPath, *_sslContext);
}

bool B1HttpsServer::implInitialize(int32 port)
{
    if (_sslContext->setServerCert(sslCert(), sslKey(), sslDh()) != true) {
        return false;
    }
    return B1HttpServer::implInitialize(port);
}
