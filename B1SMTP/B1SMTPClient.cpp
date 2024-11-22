//
// B1SMTPClient.cpp
//
// Library: B1SMTP
// Package: B1SMTP
// Module:  B1SMTP
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1SMTP.h"
#include "B1SMTPClient.h"
#include "B1SMTPClientSession.h"
#include "B1SMTPPacketMaker.h"

#include <B1Network/B1BaseSessionManager.h>

using namespace BnD;

B1SMTPClient::B1SMTPClient()
    : B1BaseClient()
    , _packetMaker(std::make_shared<B1SMTPPacketMaker>())
{
}

B1SMTPClient::~B1SMTPClient()
{
}

auto B1SMTPClient::createSession(B1ClientSocket* clientSocket, void* param) -> B1BaseClientSession*
{
    return new B1SMTPClientSession(clientSocket, this, _packetMaker.get());
}

bool B1SMTPClient::initialize(const B1String& address, uint16 port)
{
    if (startup() != true) {
        return false;
    }
    if (B1BaseClient::connectSync(address.copy(), port) == NULL) {
        return false;
    }
    return true;
}

void B1SMTPClient::finalize()
{
    shutdown();
}

bool B1SMTPClient::sendHello(const B1String& serverName)
{
    auto session = sessionManager()->getUniqueSession<B1SMTPClientSession>();
    return session ? session->sendHello(serverName) : false;
}
