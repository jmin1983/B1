//
// SECS2ClientSessionImplS1Recv.cpp
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
#include "B1SECS2MessageS1F2Readable.h"
#include "B1SECS2MessageS1F4Readable.h"
#include "B1SECS2MessageS1F14Readable.h"
#include "B1SECS2MessageS1F16Readable.h"
#include "B1SECS2MessageS1F18Readable.h"

using namespace BnD;

void B1SECS2ClientSession::recvMessageS1F0(uint16 sessionID, const std::vector<uint8>& systemBytes)
{
    onRecvMessageS1F0(sessionID, systemBytes);
}

void B1SECS2ClientSession::recvMessageS1F1(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes, const uint8* data, uint32 dataLength)
{
    /* Are You There Request(R)
    W
    */
    B1LOG("recvMessageS1F1\nAre You There Request (R)");
    onRecvMessageS1F1(sessionID, wait, systemBytes);
}

void B1SECS2ClientSession::recvMessageS1F2(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes, const uint8* data, uint32 dataLength)
{
    B1SECS2MessageS1F2Readable messageS1F2(wait, data, dataLength, secs2DataManager());
    if (messageS1F2.readData() != true) {
        return;
    }
    onRecvMessageS1F2(sessionID, wait, systemBytes);
}

void B1SECS2ClientSession::recvMessageS1F4(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes, const uint8* data, uint32 dataLength)
{
    B1SECS2MessageS1F4Readable messageS1F4(wait, data, dataLength, secs2DataManager());
    if (messageS1F4.readData() != true) {
        return;
    }
    onRecvMessageS1F4(sessionID, wait, systemBytes, messageS1F4.svs());
}

void B1SECS2ClientSession::recvMessageS1F14(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes, const uint8* data, uint32 dataLength)
{
    B1SECS2MessageS1F14Readable message(wait, data, dataLength, secs2DataManager());
    if (message.readData() != true) {
        return;
    }
    onRecvMessageS1F14(sessionID, wait, systemBytes, message.commAck(), message.mdln(), message.softRev());
}

void B1SECS2ClientSession::recvMessageS1F16(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes, const uint8* data, uint32 dataLength)
{
    B1SECS2MessageS1F16Readable message(wait, data, dataLength, secs2DataManager());
    if (message.readData() != true) {
        return;
    }
    onRecvMessageS1F16(sessionID, wait, systemBytes, message.oflAck());
}

void B1SECS2ClientSession::recvMessageS1F18(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes, const uint8* data, uint32 dataLength)
{
    B1SECS2MessageS1F18Readable message(wait, data, dataLength, secs2DataManager());
    if (message.readData() != true) {
        return;
    }
    onRecvMessageS1F18(sessionID, wait, systemBytes, message.onlAck());
}
