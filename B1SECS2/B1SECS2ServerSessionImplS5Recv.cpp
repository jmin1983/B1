//
// B1SECS2ServerSessionImplS5Recv.cpp
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
#include "B1SECS2MessageS5F2Readable.h"
#include "B1SECS2MessageS5F3Readable.h"
#include "B1SECS2MessageS5F5Readable.h"
#include "B1SECS2MessageS5F7Readable.h"

using namespace BnD;

void B1SECS2ServerSession::recvMessageS5F0(uint16 sessionID, const std::vector<uint8>& systemBytes)
{
    _t6Checker.stop();
    onRecvMessageS5F0(sessionID, systemBytes);
}

void B1SECS2ServerSession::recvMessageS5F2(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes, const uint8* data, uint32 dataLength)
{
    B1SECS2MessageS5F2Readable messageS5F2(wait, data, dataLength, secs2DataManager());
    if (messageS5F2.readData() != true) {
        writeLog("sendMessageS5F0");
        sendMessageS5F0(sessionID, systemBytes);
        return;
    }
    onRecvMessageS5F2(sessionID, systemBytes, messageS5F2.ackc5());
}

void B1SECS2ServerSession::recvMessageS5F3(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes, const uint8* data, uint32 dataLength)
{
    B1SECS2MessageS5F3Readable messageS5F3(wait, data, dataLength, secs2DataManager());
    if (messageS5F3.readData() != true) {
        writeLog("sendMessageS5F0");
        sendMessageS5F0(sessionID, systemBytes);
        return;
    }
    _t6Checker.reset();
    onRecvMessageS5F3(sessionID, wait, systemBytes, messageS5F3.alED(), messageS5F3.alID());
}

void B1SECS2ServerSession::recvMessageS5F5(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes, const uint8* data, uint32 dataLength)
{
    B1SECS2MessageS5F5Readable messageS5F5(wait, data, dataLength, secs2DataManager());
    if (messageS5F5.readData() != true) {
        writeLog("sendMessageS5F0");
        sendMessageS5F0(sessionID, systemBytes);
        return;
    }
    _t6Checker.reset();
    onRecvMessageS5F5(sessionID, wait, systemBytes, messageS5F5.alID());
}

void B1SECS2ServerSession::recvMessageS5F7(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes, const uint8* data, uint32 dataLength)
{
    B1SECS2MessageS5F7Readable messageS5F7(wait, data, dataLength);
    if (messageS5F7.readData() != true) {
        writeLog("sendMessageS5F0");
        sendMessageS5F0(sessionID, systemBytes);
        return;
    }
    _t6Checker.reset();
    onRecvMessageS5F7(sessionID, wait, systemBytes);
}
