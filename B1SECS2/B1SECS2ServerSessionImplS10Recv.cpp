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
#include "B1SECS2MessageS10F2Readable.h"
#include "B1SECS2MessageS10F3Readable.h"
#include "B1SECS2MessageS10F5Readable.h"

using namespace BnD;

void B1SECS2ServerSession::recvMessageS10F0(uint16 sessionID, const std::vector<uint8>& systemBytes)
{
    onRecvMessageS10F0(sessionID, systemBytes);
}

void B1SECS2ServerSession::recvMessageS10F2(uint16 sessionID, const std::vector<uint8>& systemBytes, const uint8* data, uint32 dataLength)
{
    _t6Checker.stop();
    B1SECS2MessageS10F2Readable messageS10F2(data, dataLength, serverSecs2DataManager());
    if (messageS10F2.readData() != true) {
        return;
    }
    onRecvMessageS10F2(sessionID, systemBytes, messageS10F2.ackc10());
}

void B1SECS2ServerSession::recvMessageS10F3(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes, const uint8* data, uint32 dataLength)
{
    B1SECS2MessageS10F3Readable messageS10F3(wait, data, dataLength, serverSecs2DataManager());
    if (messageS10F3.readData() != true) {
        return;
    }
    onRecvMessageS10F3(sessionID, systemBytes, messageS10F3.tid(), messageS10F3.text());
}

void B1SECS2ServerSession::recvMessageS10F5(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes, const uint8* data, uint32 dataLength)
{
    B1SECS2MessageS10F5Readable messageS10F5(wait, data, dataLength, serverSecs2DataManager());
    if (messageS10F5.readData() != true) {
        return;
    }
    onRecvMessageS10F5(sessionID, systemBytes, messageS10F5.tid(), messageS10F5.texts());
}
