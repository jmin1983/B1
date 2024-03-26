//
// SECS2ClientSessionImplS5Send.cpp
//
// Library: B1SECS2
// Package: Client
// Module:  B1SECS2
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1SECS2.h"
#include "B1SECS2ClientSession.h"
#include "B1SECS2MessageS5F2Writable.h"
#include "B1SECS2MessageS5F3Writable.h"

using namespace BnD;

bool B1SECS2ClientSession::sendMessageS5F2(uint16 sessionID, const B1SECS2DataACKC5& ackc5)
{
    B1SECS2MessageS5F2Writable message(ackc5);
    if (message.writeData() != true) {
        return false;
    }
    sendDataMessage(sessionID, makeNewSystemBytes(), message);
    return true;
}

bool B1SECS2ClientSession::sendMessageS5F3(const B1SECS2DataALED& alED, const B1SECS2DataALID& alID)
{
    B1SECS2MessageS5F3Writable message(alED, alID);
    if (message.writeData() != true) {
        return false;
    }
    sendDataMessage(_secs2SessionID, makeNewSystemBytes(), message);
    return true;
}
