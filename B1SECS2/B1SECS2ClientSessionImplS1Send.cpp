//
// SECS2ClientSessionImplS1Send.cpp
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
#include "B1SECS2MessageS1F2Writable.h"
#include "B1SECS2MessageS1F3Writable.h"
#include "B1SECS2MessageS1F13Writable.h"
#include "B1SECS2MessageS1F15Writable.h"

using namespace BnD;

bool B1SECS2ClientSession::sendMessageS1F2(uint16 sessionID, const std::vector<uint8>& systemBytes)
{
    B1SECS2MessageS1F2Writable messageS1F2;
    if (messageS1F2.writeData() != true) {
        B1LOG("sendMessageS1F0");
        sendMessageS1F0(sessionID, systemBytes);
        return false;
    }
    sendDataMessage(sessionID, systemBytes, messageS1F2);
    return true;
}

bool B1SECS2ClientSession::sendMessageS1F3(const std::list<B1SECS2DataSVID>& svIDs)
{
    B1SECS2MessageS1F3Writable messageS1F3(svIDs);
    if (messageS1F3.writeData() != true) {
        return false;
    }
    sendDataMessage(_secs2SessionID, makeNewSystemBytes(), messageS1F3);
    return true;
}

bool B1SECS2ClientSession::sendMessageS1F13()
{
    B1SECS2MessageS1F13Writable messageS1F13;
    if (messageS1F13.writeData() != true) {
        B1LOG("sendMessageS1F0");
        sendMessageS1F0(_secs2SessionID, makeNewSystemBytes());
        return false;
    }
    sendDataMessage(_secs2SessionID, makeNewSystemBytes(), messageS1F13);
    return true;
}

bool B1SECS2ClientSession::sendMessageS1F15()
{
    B1SECS2MessageS1F15Writable messageS1F15;
    if (messageS1F15.writeData() != true) {
        B1LOG("sendMessageS1F0");
        sendMessageS1F0(_secs2SessionID, makeNewSystemBytes());
        return false;
    }
    sendDataMessage(_secs2SessionID, makeNewSystemBytes(), messageS1F15);
    return true;
}

bool B1SECS2ClientSession::sendMessageS1F17()
{
    /* Request ON-LINE(RONL)
    W
    */
    B1LOG("sendMessageS1F17\nRequest ON-LINE (RONL)");
    sendDataMessage(_secs2SessionID, makeNewSystemBytes(), 1, 17, true);
    return true;
}
