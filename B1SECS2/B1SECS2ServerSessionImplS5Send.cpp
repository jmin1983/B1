//
// B1SECS2ServerSessionImplS5Send.cpp
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
#include "B1SECS2MessageS5F1Writable.h"
#include "B1SECS2MessageS5F4Writable.h"
#include "B1SECS2MessageS5F6Writable.h"
#include "B1SECS2MessageS5F8Writable.h"

using namespace BnD;

void B1SECS2ServerSession::sendMessageS5F1(uint16 sessionID, const std::vector<uint8>& systemBytes, const B1SECS2DataALCD& alCD, const B1SECS2DataALID& alID, const B1SECS2DataALTX& alTX)
{
    B1SECS2MessageS5F1Writable messageS5F1(alCD, alID, alTX);
    if (messageS5F1.writeData() != true) {
        writeLog("sendMessageS5F0");
        sendMessageS5F0(sessionID, systemBytes);
        return;
    }
    sendDataMessage(sessionID, systemBytes, messageS5F1);
}

void B1SECS2ServerSession::sendMessageS5F4(uint16 sessionID, const std::vector<uint8>& systemBytes, const B1SECS2DataACKC5& ackc5)
{
    _t6Checker.stop();
    B1SECS2MessageS5F4Writable messageS5F4(ackc5);
    if (messageS5F4.writeData() != true) {
        writeLog("sendMessageS5F0");
        sendMessageS5F0(sessionID, systemBytes);
        return;
    }
    sendDataMessage(sessionID, systemBytes, messageS5F4);
}

void B1SECS2ServerSession::sendMessageS5F6(uint16 sessionID, const std::vector<uint8>& systemBytes, const std::map<B1SECS2DataALID, std::pair<B1SECS2DataALCD, B1SECS2DataALTX> >& alarms)
{
    _t6Checker.stop();
    B1SECS2MessageS5F6Writable messageS5F6(alarms);
    if (messageS5F6.writeData() != true) {
        writeLog("sendMessageS5F0");
        sendMessageS5F0(sessionID, systemBytes);
        return;
    }
    sendDataMessage(sessionID, systemBytes, messageS5F6);
}

void B1SECS2ServerSession::sendMessageS5F8(uint16 sessionID, const std::vector<uint8>& systemBytes, const std::map<B1SECS2DataALID, std::pair<B1SECS2DataALCD, B1SECS2DataALTX> >& alarms)
{
    _t6Checker.stop();
    B1SECS2MessageS5F8Writable messageS5F8(alarms);
    if (messageS5F8.writeData() != true) {
        writeLog("sendMessageS5F0");
        sendMessageS5F0(sessionID, systemBytes);
        return;
    }
    sendDataMessage(sessionID, systemBytes, messageS5F8);
}
