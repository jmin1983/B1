//
// B1HttpClient.cpp
//
// Library: B1Http
// Package: Http
// Module:  B1Http
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1Http.h"
#include "B1HttpClient.h"
#include "B1HttpClientSession.h"

#include <B1Network/B1BaseSessionManager.h>

using namespace BnD;

std::shared_ptr<B1BaseClientSession> B1BaseHttpClient::getSession(int32 tagID)
{
    return sessionManager()->getSessionByHandleID<B1BaseClientSession>(tagID);
}

bool B1BaseHttpClient::initialize()
{
    return startup();
}

void B1BaseHttpClient::finalize()
{
    shutdown();
}

bool B1BaseHttpClient::connect(B1String&& address, uint16 port, int32 tagID)
{
    if (B1BaseClient::connect(std::move(address), port, NULL, tagID) == NULL) {
        return false;
    }
    return true;
}

void B1BaseHttpClient::closeSession(int32 tagID)
{
    if (auto session = getSession(tagID)) {
        session->closeSession();
    }
}

B1BaseClientSession* B1HttpClient::createSession(B1ClientSocket* clientSocket, void* param)
{
    return new B1HttpClientSession(clientSocket, this);
}

std::shared_ptr<B1HttpClientSession> B1HttpClient::getSession(int32 tagID)
{
    return sessionManager()->getSessionByHandleID<B1HttpClientSession>(tagID);
}

void B1HttpClient::sendHttpRequest(int32 tagID, const B1String& target)
{
    if (auto session = getSession(tagID)) {
        session->sendHttpRequest(target);
    }
}
