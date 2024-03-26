//
// SECS2ClientSessionImplS5Recv.cpp
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
#include "B1SECS2MessageS5F1Readable.h"
#include "B1SECS2MessageS5F4Readable.h"

using namespace BnD;

void B1SECS2ClientSession::recvMessageS5F1(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes, const uint8* data, uint32 dataLength)
{
    B1SECS2MessageS5F1Readable message(wait, data, dataLength, secs2DataManager());
    if (message.readData() != true) {
        return;
    }
    onRecvMessageS5F1(sessionID, wait, systemBytes, message.alCD(), message.alID(), message.alTX());
}
void B1SECS2ClientSession::recvMessageS5F4(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes, const uint8* data, uint32 dataLength)
{
    B1SECS2MessageS5F4Readable message(wait, data, dataLength, secs2DataManager());
    if (message.readData() != true) {
        return;
    }
    onRecvMessageS5F4(sessionID, wait, systemBytes, message.ackc5());
}