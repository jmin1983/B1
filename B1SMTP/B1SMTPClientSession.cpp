//
// B1SMTPClientSession.cpp
//
// Library: B1SMTP
// Package: B1SMTP
// Module:  B1SMTP
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1SMTP.h"
#include "B1SMTPClientSession.h"
#include "B1SMTPPacketMaker.h"
#include "B1SMTPProtocol.h"

using namespace BnD;

B1SMTPClientSession::B1SMTPClientSession(B1ClientSocket* clientSocket, B1BaseClientSessionListener* listener, B1SMTPPacketMaker* packetMaker)
    : B1ArrayBufferClientSession(clientSocket, listener)
    , _packetMaker(packetMaker)
    , _remoteServiceReady(false)
{
}

B1SMTPClientSession::~B1SMTPClientSession()
{
}

void B1SMTPClientSession::implOnRecvSMTPResponseServiceReady(B1String&& message)
{
    _remoteServiceReady = true;
}

void B1SMTPClientSession::onReadComplete(uint8* data, size_t dataSize)
{
    if (analyzeData(data, dataSize) != true) {
        disconnect();
    }
}

void B1SMTPClientSession::implOnConnect()
{
    B1LOG("session connected -> clear buffer");
    clearBuffer();
    B1ArrayBufferClientSession::implOnConnect();
}

void B1SMTPClientSession::implOnDisconnected(int32 reason)
{
    B1LOG("session disconnected");
    B1ArrayBufferClientSession::implOnDisconnected(reason);
}

bool B1SMTPClientSession::sendHello(const B1String& serverName)
{
    if (isRemoteServiceReady() != true) {
        B1LOG("remote service is not ready");
        return false;
    }
    auto data = _packetMaker->makeDataHello(serverName);
    return writeData(data);
}
