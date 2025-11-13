//
// SECS2ClientSessionImplS2Recv.cpp
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
#include "B1SECS2MessageS2F32Readable.h"
#include "B1SECS2MessageS2F34Readable.h"
#include "B1SECS2MessageS2F36Readable.h"
#include "B1SECS2MessageS2F38Readable.h"
#include "B1SECS2MessageS2F42Readable.h"
#include "B1SECS2MessageS2F50Readable.h"

using namespace BnD;

void B1SECS2ClientSession::recvMessageS2F0(uint16 sessionID, const std::vector<uint8>& systemBytes)
{
    onRecvMessageS2F0(sessionID, systemBytes);
}

void B1SECS2ClientSession::recvMessageS2F32(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes, const uint8* data, uint32 dataLength)
{
    B1SECS2MessageS2F32Readable message(wait, data, dataLength, secs2DataManager());
    if (message.readData() != true) {
        return;
    }
    onRecvMessageS2F32(sessionID, systemBytes, message.tiAck());
}

void B1SECS2ClientSession::recvMessageS2F34(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes, const uint8* data, uint32 dataLength)
{
    B1SECS2MessageS2F34Readable message(wait, data, dataLength, secs2DataManager());
    if (message.readData() != true) {
        return;
    }
    onRecvMessageS2F34(sessionID, systemBytes, message.drAck());
}
void B1SECS2ClientSession::recvMessageS2F36(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes, const uint8* data, uint32 dataLength)
{
    B1SECS2MessageS2F36Readable message(wait, data, dataLength, secs2DataManager());
    if (message.readData() != true) {
        return;
    }
    onRecvMessageS2F36(sessionID, systemBytes, message.lrAck());
}
void B1SECS2ClientSession::recvMessageS2F38(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes, const uint8* data, uint32 dataLength)
{
    B1SECS2MessageS2F38Readable message(wait, data, dataLength, secs2DataManager());
    if (message.readData() != true) {
        return;
    }
    onRecvMessageS2F38(sessionID, systemBytes, message.erAck());
}

void B1SECS2ClientSession::recvMessageS2F42(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes, const uint8* data, uint32 dataLength)
{
    B1SECS2MessageS2F42Readable message(wait, data, dataLength, secs2DataManager());
    if (message.readData() != true) {
        return;
    }
    onRecvMessageS2F42(sessionID, systemBytes, message.hcAck(), message.cps());
}

void B1SECS2ClientSession::recvMessageS2F50(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes, const uint8* data, uint32 dataLength)
{
    B1SECS2MessageS2F50Readable message(wait, data, dataLength, secs2DataManager());
    if (message.readData() != true) {
        return;
    }
    onRecvMessageS2F50(sessionID, systemBytes, message.hcAck(), message.cps());
}
