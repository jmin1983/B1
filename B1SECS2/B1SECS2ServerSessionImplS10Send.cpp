//
// B1SECS2ServerSessionImplS1Send.cpp
//
// Library: B1SECS2
// Package: Server
// Module:  B1SECS2
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1SECS2.h"
#include "B1SECS2ServerSession.h"
#include "B1SECS2MessageS10F1Writable.h"
#include "B1SECS2MessageS10F4Writable.h"
#include "B1SECS2MessageS10F6Writable.h"

using namespace BnD;

void B1SECS2ServerSession::sendMessageS10F1(uint16 sessionID, const std::vector<uint8>& systemBytes, const B1SECS2DataTID& tid, const B1SECS2DataTEXT& text)
{
    B1SECS2MessageS10F1Writable messageS10F1(tid, text);
    if (messageS10F1.writeData() != true) {
        B1LOG("sendMessageS10F0");
        sendMessageS10F0(sessionID, systemBytes);
        return;
    }
    _t6Checker.reset();
    sendDataMessage(sessionID, systemBytes, messageS10F1);
}

void B1SECS2ServerSession::sendMessageS10F4(uint16 sessionID, const std::vector<uint8>& systemBytes, const B1SECS2DataACKC10& ackc10)
{
    B1SECS2MessageS10F4Writable messageS10F4(ackc10);
    if (messageS10F4.writeData() != true) {
        B1LOG("sendMessageS10F0");
        sendMessageS10F0(sessionID, systemBytes);
        return;
    }
    sendDataMessage(sessionID, systemBytes, messageS10F4);
}

void B1SECS2ServerSession::sendMessageS10F6(uint16 sessionID, const std::vector<uint8>& systemBytes, const B1SECS2DataACKC10& ackc10)
{
    B1SECS2MessageS10F6Writable messageS10F6(ackc10);
    if (messageS10F6.writeData() != true) {
        B1LOG("sendMessageS10F0");
        sendMessageS10F0(sessionID, systemBytes);
        return;
    }
    sendDataMessage(sessionID, systemBytes, messageS10F6);
}

