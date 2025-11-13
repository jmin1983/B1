//
// B1SECS2ServerSessionImplS6Recv.cpp
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
#include "B1SECS2MessageS6F12Readable.h"
#include "B1SECS2MessageS6F15Readable.h"
#include "B1SECS2MessageS6F19Readable.h"

using namespace BnD;

void B1SECS2ServerSession::recvMessageS6F0(uint16 sessionID, const std::vector<uint8>& systemBytes)
{
    _t6Checker.stop();
    onRecvMessageS6F0(sessionID, systemBytes);
}

void B1SECS2ServerSession::recvMessageS6F12(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes, const uint8* data, uint32 dataLength)
{
    _t6Checker.stop();
    B1SECS2MessageS6F12Readable messageS6F12(wait, data, dataLength, secs2DataManager());
    if (messageS6F12.readData() != true) {
        return;
    }
    onRecvMessageS6F12(sessionID, systemBytes, messageS6F12.ackc6());
}

void B1SECS2ServerSession::recvMessageS6F15(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes, const uint8* data, uint32 dataLength)
{
    B1SECS2MessageS6F15Readable messageS6F15(wait, data, dataLength, secs2DataManager());
    if (messageS6F15.readData() != true) {
        writeLog("sendMessageS6F0");
        sendMessageS6F0(sessionID, systemBytes);
        return;
    }
    onRecvMessageS6F15(sessionID, wait, systemBytes, messageS6F15.ceID());
}

void B1SECS2ServerSession::recvMessageS6F19(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes, const uint8* data, uint32 dataLength)
{
    B1SECS2MessageS6F19Readable messageS6F19(wait, data, dataLength, secs2DataManager());
    if (messageS6F19.readData() != true) {
        writeLog("sendMessageS6F0");
        sendMessageS6F0(sessionID, systemBytes);
        return;
    }
    onRecvMessageS6F19(sessionID, wait, systemBytes, messageS6F19.rptID());
}
