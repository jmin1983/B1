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
#include "B1SECS2MessageS3F17Readable.h"

using namespace BnD;

void B1SECS2ServerSession::recvMessageS3F0(uint16 sessionID, const std::vector<uint8>& systemBytes)
{
    onRecvMessageS3F0(sessionID, systemBytes);
}

void B1SECS2ServerSession::recvMessageS3F17(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes, const uint8* data, uint32 dataLength)
{
    B1SECS2MessageS3F17Readable messageS3F17(wait, data, dataLength, serverSecs2DataManager());
    if (messageS3F17.readData() != true) {
        B1LOG("sendMessageS3F0");
        sendMessageS3F0(sessionID, systemBytes);
        return;
    }
    onRecvMessageS3F17(sessionID, wait, systemBytes, messageS3F17.dataID(), messageS3F17.carrierAction(), messageS3F17.carrierID(), messageS3F17.ptn(), messageS3F17.cattrData());
}
