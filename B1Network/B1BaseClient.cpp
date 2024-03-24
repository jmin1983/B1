//
// B1BaseClient.cpp
//
// Library: B1Network
// Package: B1Network
// Module:  B1Network
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1Network.h"
#include "B1BaseClient.h"
#include "B1BaseClientSession.h"
#include "B1BaseSessionManager.h"
#include "B1ClientSocket.h"

using namespace BnD;

B1BaseClient::B1BaseClient()
    : _waiterForConnectSync(false)
{
}

B1BaseClient::~B1BaseClient()
{
}

void B1BaseClient::onClientSessionConnectResult(B1ClientSocket* clientSocket, bool isConnected, int32 reason)
{
    _waiterForConnectSync = true;
}

void B1BaseClient::onClientSessionClosed(B1ClientSocket* clientSocket)
{
    B1LOG("close connection");
    sessionManager()->reserveRemoveSession(clientSocket);
}

B1ClientSocket* B1BaseClient::connect(B1String&& address, uint16 port, void* param, int32 requestedHandleID)
{
    auto socket = std::shared_ptr<B1ClientSocket>(new B1ClientSocket());
    auto session = std::shared_ptr<B1BaseClientSession>(createSession(socket.get(), param));
    if (session->initialize() != true) {
        B1LOG("initialize session failed");
        assert(false);
        return NULL;
    }
    int32 handleID = sessionManager()->addSession(socket, session, requestedHandleID);
    if (handleID < 0) {
        return NULL;
    }
    session->setSessionHandleID(handleID);
    if (session->connect(std::move(address), port) != true) {
        sessionManager()->reserveRemoveSession(socket.get());
        return NULL;
    }
    return socket.get();
}

B1ClientSocket* B1BaseClient::connectSync(B1String&& address, uint16 port, void* param, int32 requestedHandleID)
{
    _waiterForConnectSync = false;
    auto result = connect(std::move(address), port, param, requestedHandleID);
    if (NULL == result) {
        return NULL;
    }
    while (_waiterForConnectSync != true) {
        B1Thread::sleep(10);
    }
    if (result->isOpen() != true) {
        closeSession(result);
        return NULL;
    }
    return result;
}

void B1BaseClient::disconnect(B1ClientSocket* handle)
{
    if (auto session = sessionManager()->getBaseSession(handle)) {
        session->disconnect();
    }
}

void B1BaseClient::disconnectByHandleID(int32 handleID)
{
    if (auto session = sessionManager()->getBaseSessionByHandleID(handleID)) {
        session->disconnect();
    }
}

void B1BaseClient::closeSession(B1ClientSocket* handle)
{
    if (auto session = sessionManager()->getSession<B1BaseClientSession>(handle)) {
        session->closeSession();
    }
}

bool B1BaseClient::isConnected(B1ClientSocket* handle) const
{
    if (auto session = sessionManager()->getBaseSession(handle)) {
        return session->isConnected();
    }
    return false;
}

int32 B1BaseClient::currentConnectionStatus(B1ClientSocket* handle) const
{
    if (auto session = sessionManager()->getBaseSession(handle)) {
        return session->currentConnectionStatus();
    }
    return -1;
}
