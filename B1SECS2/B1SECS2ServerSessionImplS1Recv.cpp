//
// B1SECS2ServerSessionImplS1Recv.cpp
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
#include "B1SECS2MessageS1F2Readable.h"
#include "B1SECS2MessageS1F3Readable.h"
#include "B1SECS2MessageS1F11Readable.h"
#include "B1SECS2MessageS1F13Readable.h"
#include "B1SECS2MessageS1F14Readable.h"
#include "B1SECS2MessageS1F21Readable.h"
#include "B1SECS2MessageS1F23Readable.h"

using namespace BnD;

void B1SECS2ServerSession::recvMessageS1F0(uint16 sessionID, const std::vector<uint8>& systemBytes)
{
    _t6Checker.stop();
    onRecvMessageS1F0(sessionID, systemBytes);
}

void B1SECS2ServerSession::recvMessageS1F1(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes, const uint8* data, uint32 dataLength)
{
    /* Are You There Request(R)
    W
    */
    writeLog("recvMessageS1F1\nAre You There Request (R)");
    onRecvMessageS1F1(sessionID, wait, systemBytes);
}

void B1SECS2ServerSession::recvMessageS1F2(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes, const uint8* data, uint32 dataLength)
{
    _t6Checker.stop();
    B1SECS2MessageS1F2Readable messageS1F2(wait, data, dataLength, secs2DataManager());
    if (messageS1F2.readData() != true)
        return;
    onRecvMessageS1F2(sessionID, wait, systemBytes);
}

void B1SECS2ServerSession::recvMessageS1F3(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes, const uint8* data, uint32 dataLength)
{
    B1SECS2MessageS1F3Readable messageS1F3(wait, data, dataLength, secs2DataManager());
    if (messageS1F3.readData() != true) {
        writeLog("sendMessageS1F0");
        sendMessageS1F0(sessionID, systemBytes);
        return;
    }
    onRecvMessageS1F3(sessionID, wait, systemBytes, messageS1F3.svIDs());
}

void B1SECS2ServerSession::recvMessageS1F11(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes, const uint8* data, uint32 dataLength)
{
    B1SECS2MessageS1F11Readable messageS1F11(wait, data, dataLength, secs2DataManager());
    if (messageS1F11.readData() != true) {
        writeLog("sendMessageS1F0");
        sendMessageS1F0(sessionID, systemBytes);
        return;
    }
    onRecvMessageS1F11(sessionID, wait, systemBytes, messageS1F11.svIDs());
}

void B1SECS2ServerSession::recvMessageS1F13(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes, const uint8* data, uint32 dataLength)
{
    B1SECS2MessageS1F13Readable messageS1F13(wait, data, dataLength, secs2DataManager());
    if (messageS1F13.readData() != true) {
        writeLog("sendMessageS1F0");
        sendMessageS1F0(sessionID, systemBytes);
        return;
    }
    onRecvMessageS1F13(sessionID, wait, systemBytes);
}

void B1SECS2ServerSession::recvMessageS1F14(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes, const uint8* data, uint32 dataLength)
{
    B1SECS2MessageS1F14Readable messageS1F14(wait, data, dataLength, secs2DataManager());
    if (messageS1F14.readData() != true) {
        writeLog("sendMessageS1F0");
        sendMessageS1F0(sessionID, systemBytes);
        return;
    }

    onRecvMessageS1F14(sessionID, wait, systemBytes, messageS1F14.commAck());
}

void B1SECS2ServerSession::recvMessageS1F15(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes, const uint8* data, uint32 dataLength)
{
    /* Request OFF-LINE(ROFL)
    W
    */
    writeLog("recvMessageS1F15\nRequest OFF-LINE (ROFL)");
    onRecvMessageS1F15(sessionID, wait, systemBytes);
}

void B1SECS2ServerSession::recvMessageS1F17(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes, const uint8* data, uint32 dataLength)
{
    /* Request ON-LINE(RONL)
    W
    */
    writeLog("recvMessageS1F17\nRequest ON-LINE (RONL)");
    onRecvMessageS1F17(sessionID, wait, systemBytes);
}

void B1SECS2ServerSession::recvMessageS1F21(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes, const uint8* data, uint32 dataLength)
{
    B1SECS2MessageS1F21Readable messageS1F21(wait, data, dataLength, secs2DataManager());
    if (messageS1F21.readData() != true) {
        writeLog("sendMessageS1F0");
        sendMessageS1F0(sessionID, systemBytes);
        return;
    }
    onRecvMessageS1F21(sessionID, wait, systemBytes, messageS1F21.vIDs());
}

void B1SECS2ServerSession::recvMessageS1F23(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes, const uint8* data, uint32 dataLength)
{
    B1SECS2MessageS1F23Readable messageS1F23(wait, data, dataLength, secs2DataManager());
    if (messageS1F23.readData() != true) {
        writeLog("sendMessageS1F0");
        sendMessageS1F0(sessionID, systemBytes);
        return;
    }
    onRecvMessageS1F23(sessionID, wait, systemBytes, messageS1F23.ceIDs());
}
