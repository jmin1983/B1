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
#include "B1SECS2MessageS3F18Readable.h"

using namespace BnD;

void B1SECS2ClientSession::recvMessageS3F0(const std::vector<uint8>& systemBytes)
{
    onRecvMessageS3F0(systemBytes);
}

void B1SECS2ClientSession::recvMessageS3F18(bool wait, const std::vector<uint8>& systemBytes, const uint8* data, uint32 dataLength)
{
    B1SECS2MessageS3F18Readable message(wait, data, dataLength, clientSecs2DataManager());
    if (message.readData() != true) {
        return;
    }
    onRecvMessageS3F18(systemBytes, message.acAck(), message.errorText());
}
