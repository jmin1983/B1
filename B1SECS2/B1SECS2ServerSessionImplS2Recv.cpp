//
// B1SECS2ServerSessionImplS2Recv.cpp
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
#include "B1SECS2MessageS2F13Readable.h"
#include "B1SECS2MessageS2F15Readable.h"
#include "B1SECS2MessageS2F29Readable.h"
#include "B1SECS2MessageS2F31Readable.h"
#include "B1SECS2MessageS2F33Readable.h"
#include "B1SECS2MessageS2F35Readable.h"
#include "B1SECS2MessageS2F37Readable.h"
#include "B1SECS2MessageS2F41Readable.h"
#include "B1SECS2MessageS2F49Readable.h"

using namespace BnD;

void B1SECS2ServerSession::recvMessageS2F0(uint16 sessionID, const std::vector<uint8>& systemBytes)
{
    onRecvMessageS2F0(sessionID, systemBytes);
}

void B1SECS2ServerSession::recvMessageS2F13(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes, const uint8* data, uint32 dataLength)
{
    B1SECS2MessageS2F13Readable messageS2F13(wait, data, dataLength, secs2DataManager());
    if (messageS2F13.readData() != true) {
        B1LOG("sendMessageS2F0");
        sendMessageS2F0(sessionID, systemBytes);
        return;
    }
    onRecvMessageS2F13(sessionID, wait, systemBytes, messageS2F13.ecIDs());
}

void B1SECS2ServerSession::recvMessageS2F15(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes, const uint8* data, uint32 dataLength)
{
    B1SECS2MessageS2F15Readable messageS2F15(wait, data, dataLength, secs2DataManager());
    if (messageS2F15.readData() != true) {
        B1LOG("sendMessageS2F0");
        sendMessageS2F0(sessionID, systemBytes);
        return;
    }
    onRecvMessageS2F15(sessionID, wait, systemBytes, messageS2F15.ecvs());
}

void B1SECS2ServerSession::recvMessageS2F17(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes, const uint8* data, uint32 dataLength)
{
    /*  Date and Time Request(DTR)
    W
    */
    B1LOG("recvMessageS2F17\nDate and Time Request (DTR)");
    onRecvMessageS2F17(sessionID, wait, systemBytes);
}

void B1SECS2ServerSession::recvMessageS2F29(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes, const uint8* data, uint32 dataLength)
{
    B1SECS2MessageS2F29Readable messageS2F29(wait, data, dataLength, secs2DataManager());
    if (messageS2F29.readData() != true) {
        B1LOG("sendMessageS2F0");
        sendMessageS2F0(sessionID, systemBytes);
        return;
    }
    onRecvMessageS2F29(sessionID, wait, systemBytes, messageS2F29.ecIDs());
}

void B1SECS2ServerSession::recvMessageS2F31(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes, const uint8* data, uint32 dataLength)
{
    B1SECS2MessageS2F31Readable messageS2F31(wait, data, dataLength, secs2DataManager());
    if (messageS2F31.readData() != true) {
        B1LOG("sendMessageS2F0");
        sendMessageS2F0(sessionID, systemBytes);
        return;
    }
    onRecvMessageS2F31(sessionID, wait, systemBytes, messageS2F31.dateTime());
}

void B1SECS2ServerSession::recvMessageS2F33(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes, const uint8* data, uint32 dataLength)
{
    B1SECS2MessageS2F33Readable messageS2F33(wait, data, dataLength, secs2DataManager());
    if (messageS2F33.readData() != true) {
        B1LOG("sendMessageS2F0");
        sendMessageS2F0(sessionID, systemBytes);
        return;
    }
    onRecvMessageS2F33(sessionID, wait, systemBytes, messageS2F33.dataID(), messageS2F33.reportData());
}

void B1SECS2ServerSession::recvMessageS2F35(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes, const uint8* data, uint32 dataLength)
{
    B1SECS2MessageS2F35Readable messageS2F35(wait, data, dataLength, secs2DataManager());
    if (messageS2F35.readData() != true) {
        B1LOG("sendMessageS2F0");
        sendMessageS2F0(sessionID, systemBytes);
        return;
    }
    onRecvMessageS2F35(sessionID, wait, systemBytes, messageS2F35.dataID(), messageS2F35.linkEventData());
}

void B1SECS2ServerSession::recvMessageS2F37(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes, const uint8* data, uint32 dataLength)
{
    B1SECS2MessageS2F37Readable messageS2F37(wait, data, dataLength, secs2DataManager());
    if (messageS2F37.readData() != true) {
        B1LOG("sendMessageS2F0");
        sendMessageS2F0(sessionID, systemBytes);
        return;
    }
    onRecvMessageS2F37(sessionID, wait, systemBytes, messageS2F37.ceed(), messageS2F37.ceIDs());
}

void B1SECS2ServerSession::recvMessageS2F41(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes, const uint8* data, uint32 dataLength)
{
    B1SECS2MessageS2F41Readable messageS2F41(wait, data, dataLength, secs2DataManager());
    if (messageS2F41.readData() != true) {
        B1LOG("sendMessageS2F0");
        sendMessageS2F0(sessionID, systemBytes);
        return;
    }
    onRecvMessageS2F41(sessionID, wait, systemBytes, messageS2F41.rcmd(), messageS2F41.cps());
}

void B1SECS2ServerSession::recvMessageS2F49(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes, const uint8* data, uint32 dataLength)
{
    B1SECS2MessageS2F49Readable messageS2F49(wait, data, dataLength, secs2DataManager());
    if (messageS2F49.readData() != true) {
        B1LOG("sendMessageS2F0");
        sendMessageS2F0(sessionID, systemBytes);
        return;
    }
    onRecvMessageS2F49(sessionID, wait, systemBytes, messageS2F49.dataID(), messageS2F49.rcmd(), messageS2F49.cps());
}
