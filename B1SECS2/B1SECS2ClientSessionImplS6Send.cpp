//
// SECS2ClientSessionImplS6Send.cpp
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
#include "B1SECS2MessageS6F12Writable.h"
#include "B1SECS2MessageS6F15Writable.h"
#include "B1SECS2MessageS6F19Writable.h"

using namespace BnD;

bool B1SECS2ClientSession::sendMessageS6F12(uint16 sessionID, const B1SECS2DataACKC6& ackc6)
{
    B1SECS2MessageS6F12Writable message(ackc6);
    if (message.writeData() != true) {
        return false;
    }
    sendDataMessage(sessionID, makeNewSystemBytes(), message);
    return true;
}

bool B1SECS2ClientSession::sendMessageS6F15(const B1SECS2DataCEID& ceID)
{
    B1SECS2MessageS6F15Writable message(ceID);
    if (message.writeData() != true) {
        return false;
    }
    sendDataMessage(_secs2SessionID, makeNewSystemBytes(), message);
    return true;
}

bool B1SECS2ClientSession::sendMessageS6F19(const B1SECS2DataRPTID& rptID)
{
    B1SECS2MessageS6F19Writable message(rptID);
    if (message.writeData() != true) {
        return false;
    }
    sendDataMessage(_secs2SessionID, makeNewSystemBytes(), message);
    return true;
}
