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
    , _startMailInput(false)
    , _remoteServiceClosed(false)
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

void B1SMTPClientSession::implOnRecvSMTPResponseStartMailInput(B1String&& message)
{
    _startMailInput = true;
}

void B1SMTPClientSession::implOnRecvSMTPResponseServiceClosing(B1String&& message)
{
    _remoteServiceClosed = true;
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
    _startMailInput = false;
    _remoteServiceClosed = false;
    B1ArrayBufferClientSession::implOnDisconnected(reason);
}

bool B1SMTPClientSession::waitResponse(const bool& value) const
{
    auto now = B1TickUtil::currentTick();
    while (value != true) {
        if (B1TickUtil::diffTick(now, B1TickUtil::currentTick()) > CONSTS_RESPONSE_TIME_OUT) {
            B1LOG("remote response timedout");
            return false;
        }
        B1Thread::sleep(10);
    }
    return true;
}

bool B1SMTPClientSession::waitRemoteServiceReady() const
{
    return waitResponse(_remoteServiceReady);
}

bool B1SMTPClientSession::sendAndWait(const std::vector<uint8>& data, bool* condition) const
{
    *condition = false;
    if (writeData(data) != true) {
        return false;
    }
    if (waitResponse(*condition) != true) {
        return false;
    }
    return true;
}

bool B1SMTPClientSession::sendHello(const B1String& serverName)
{
    if (waitRemoteServiceReady() != true) {
        return false;
    }
    return sendAndWait(_packetMaker->makeDataHello(serverName), &_lastActionOk);
}

bool B1SMTPClientSession::sendMailFrom(const B1Mail& mail)
{
    return sendAndWait(_packetMaker->makeDataMailFrom(mail.sender().emailAddress()), &_lastActionOk);
}

bool B1SMTPClientSession::sendRcptTO(const B1Mail& mail)
{
    for (const auto& receiver : mail.receivers()) {
        if (sendAndWait(_packetMaker->makeDataRcptTO(receiver.emailAddress()), &_lastActionOk) != true) {
            return false;
        }
    }
    return true;
}

bool B1SMTPClientSession::sendRcptCC(const B1Mail& mail)
{
    for (const auto& cc : mail.ccs()) {
        if (sendAndWait(_packetMaker->makeDataRcptCC(cc.emailAddress()), &_lastActionOk) != true) {
            return false;
        }
    }
    return true;
}

bool B1SMTPClientSession::sendRcptBCC(const B1Mail& mail)
{
    for (const auto& bcc : mail.bccs()) {
        if (sendAndWait(_packetMaker->makeDataRcptBCC(bcc.emailAddress()), &_lastActionOk) != true) {
            return false;
        }
    }
    return true;
}

bool B1SMTPClientSession::sendStartMailInput()
{
    return sendAndWait(_packetMaker->makeDataStartMailInput(), &_startMailInput);
}

bool B1SMTPClientSession::sendContents(const B1Mail& mail)
{
    return sendAndWait(_packetMaker->makeDataContents(mail), &_lastActionOk);
}

bool B1SMTPClientSession::sendQuit()
{
    return writeData(_packetMaker->makeDataQuit());
}
