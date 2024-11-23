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
#include "B1Mail.h"
#include "B1SMTPPacketMaker.h"
#include "B1SMTPProtocol.h"

#include <B1Base/B1TickUtil.h>
#include <B1Base/B1Thread.h>

using namespace BnD;

B1SMTPClientSession::B1SMTPClientSession(B1ClientSocket* clientSocket, B1BaseClientSessionListener* listener, B1SMTPPacketMaker* packetMaker)
    : B1ArrayBufferClientSession(clientSocket, listener)
    , _remoteServiceReady(false)
    , _lastActionOk(false)
    , _packetMaker(packetMaker)
{
}

B1SMTPClientSession::~B1SMTPClientSession()
{
}

void B1SMTPClientSession::implOnRecvSMTPResponseServiceReady(B1String&& message)
{
    _remoteServiceReady = true;
}

void B1SMTPClientSession::implOnRecvSMTPResponseActionOK(B1String&& message)
{
    _lastActionOk = true;
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
    _remoteServiceReady = false;
    _lastActionOk = false;
    B1ArrayBufferClientSession::implOnDisconnected(reason);
}

bool B1SMTPClientSession::sendHello(const B1String& serverName)
{
    {
        auto now = B1TickUtil::currentTick();
        while (_remoteServiceReady != true) {
            if (B1TickUtil::diffTick(now, B1TickUtil::currentTick()) > CONSTS_RESPONSE_TIME_OUT) {
                B1LOG("remote service ready timedout");
                return false;
            }
            B1Thread::sleep(10);
        }
    }
    _lastActionOk = false;
    auto data = _packetMaker->makeDataHello(serverName);
    if (writeData(data) != true) {
        return false;
    }
    {
        auto now = B1TickUtil::currentTick();
        while (_lastActionOk != true) {
            if (B1TickUtil::diffTick(now, B1TickUtil::currentTick()) > CONSTS_RESPONSE_TIME_OUT) {
                B1LOG("remote response timedout");
                return false;
            }
            B1Thread::sleep(10);
        }
    }
    return true;
}

bool B1SMTPClientSession::sendMessage(const B1Mail& mail)
{
    return true;
}
