//
// B1WebSocketClient.cpp
//
// Library: B1Http
// Package: Http
// Module:  B1Http
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1Http.h"
#include "B1WebSocketClient.h"
#include "B1WebSocketClientSession.h"

#include <B1Network/B1BaseSessionManager.h>

using namespace BnD;

B1BaseClientSession* B1WebSocketClient::createSession(B1ClientSocket* clientSocket, void* param)
{
    return new B1WebSocketClientSession(clientSocket, this);
}

std::shared_ptr<B1WebSocketClientSession> B1WebSocketClient::getSession(int32 tagID)
{
    return sessionManager()->getSessionByHandleID<B1WebSocketClientSession>(tagID);
}

bool B1WebSocketClient::sendBinary(int32 tagID, std::vector<uint8>&& data)
{
    if (auto session = getSession(tagID)) {
        session->writeBinary(std::move(data));
        return true;
    }
    return false;
}

bool B1WebSocketClient::sendText(int32 tagID, B1String&& text)
{
    if (auto session = getSession(tagID)) {
        session->writeText(std::move(text));
        return true;
    }
    return false;
}
