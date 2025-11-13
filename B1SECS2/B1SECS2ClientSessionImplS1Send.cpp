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
#include "B1SECS2MessageS1F11Writable.h"
#include "B1SECS2MessageS1F13Writable.h"
#include "B1SECS2MessageS1F14Writable.h"
#include "B1SECS2MessageS1F15Writable.h"
#include "B1SECS2MessageS1F21Writable.h"
#include "B1SECS2MessageS1F23Writable.h"

using namespace BnD;

bool B1SECS2ClientSession::sendMessageS1F1(uint16 sessionID, const std::vector<uint8>& systemBytes)
{
    B1LOG("sendMessageS1F1\nAre You There Request (R)");
    sendDataMessage(sessionID, systemBytes, 1, 1, true);
    return true;
}

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

bool B1SECS2ClientSession::sendMessageS1F11(const std::list<B1SECS2DataSVID>& svIDs)
{
    B1SECS2MessageS1F11Writable messageS1F11(svIDs);
    if (messageS1F11.writeData() != true) {
        B1LOG("sendMessageS1F0");
        sendMessageS1F0(_secs2SessionID, makeNewSystemBytes());
        return false;
    }
    sendDataMessage(_secs2SessionID, makeNewSystemBytes(), messageS1F11);
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

bool B1SECS2ClientSession::sendMessageS1F14(const B1SECS2DataCOMMACK& commAck)
{
    B1SECS2MessageS1F14Writable messageS1F14(commAck);
    if (messageS1F14.writeData() != true) {
        B1LOG("sendMessageS1F0");
        sendMessageS1F0(_secs2SessionID, makeNewSystemBytes());
        return false;
    }
    sendDataMessage(_secs2SessionID, makeNewSystemBytes(), messageS1F14);
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

bool B1SECS2ClientSession::sendMessageS1F21(const std::list<B1SECS2DataVID>& vIDs)
{
    B1SECS2MessageS1F21Writable messageS1F21(vIDs);
    if (messageS1F21.writeData() != true) {
        B1LOG("sendMessageS1F0");
        sendMessageS1F0(_secs2SessionID, makeNewSystemBytes());
        return false;
    }
    sendDataMessage(_secs2SessionID, makeNewSystemBytes(), messageS1F21);
    return true;
}

bool B1SECS2ClientSession::sendMessageS1F23(const std::list<B1SECS2DataCEID>& ceIDs)
{
    B1SECS2MessageS1F23Writable messageS1F23(ceIDs);
    if (messageS1F23.writeData() != true) {
        B1LOG("sendMessageS1F0");
        sendMessageS1F0(_secs2SessionID, makeNewSystemBytes());
        return false;
    }
    sendDataMessage(_secs2SessionID, makeNewSystemBytes(), messageS1F23);
    return true;
}
