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
