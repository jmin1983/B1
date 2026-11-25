//
// B1SECS2ClientSessionImplS10Recv.cpp
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
#include "B1SECS2MessageS10F1Readable.h"
#include "B1SECS2MessageS10F4Readable.h"
#include "B1SECS2MessageS10F6Readable.h"

using namespace BnD;

void B1SECS2ClientSession::recvMessageS10F0(const std::vector<uint8>& systemBytes)
{
    onRecvMessageS10F0(systemBytes);
}

void B1SECS2ClientSession::recvMessageS10F1(bool wait, const std::vector<uint8>& systemBytes, const uint8* data, uint32 dataLength)
{
    B1SECS2MessageS10F1Readable messageS10F1(wait, data, dataLength, clientSecs2DataManager());
    if (messageS10F1.readData() != true) {
        sendMessageS10F0(systemBytes);
        return;
    }
    onRecvMessageS10F1(wait, systemBytes, messageS10F1.tid(), messageS10F1.text());
}

void B1SECS2ClientSession::recvMessageS10F4(const std::vector<uint8>& systemBytes, const uint8* data, uint32 dataLength)
{
    B1SECS2MessageS10F4Readable messageS10F4(data, dataLength, clientSecs2DataManager());
    if (messageS10F4.readData() != true) {
        B1LOG("read S10F4 failed");     //  secondary message: no reply.
        return;
    }
    onRecvMessageS10F4(systemBytes, messageS10F4.ackc10());
}

void B1SECS2ClientSession::recvMessageS10F6(const std::vector<uint8>& systemBytes, const uint8* data, uint32 dataLength)
{
    B1SECS2MessageS10F6Readable messageS10F6(data, dataLength, clientSecs2DataManager());
    if (messageS10F6.readData() != true) {
        B1LOG("read S10F6 failed");     //  secondary message: no reply.
        return;
    }
    onRecvMessageS10F6(systemBytes, messageS10F6.ackc10());
}
