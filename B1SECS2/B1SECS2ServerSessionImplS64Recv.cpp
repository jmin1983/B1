//
// B1SECS2ServerSessionImplS64Recv.cpp
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
#include "B1SECS2MessageS64F1Readable.h"

using namespace BnD;

void B1SECS2ServerSession::recvMessageS64F0(uint16 sessionID, const std::vector<uint8>& systemBytes)
{
    onRecvMessageS64F0(sessionID, systemBytes);
}

void B1SECS2ServerSession::recvMessageS64F1(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes, const uint8* data, uint32 dataLength)
{
    B1SECS2MessageS64F1Readable messageS64F1(wait, data, dataLength, secs2DataManager());
    if (messageS64F1.readData() != true) {
        writeLog("sendMessageS64F0");
        sendMessageS64F0(sessionID, systemBytes);
        return;
    }
    onRecvMessageS64F1(sessionID, wait, systemBytes, messageS64F1.rcmd(), messageS64F1.cps());
}
