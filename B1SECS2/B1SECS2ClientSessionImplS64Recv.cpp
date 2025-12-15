//
// SECS2ClientSessionImplS64Recv.cpp
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
#include "B1SECS2MessageS64F2Readable.h"

using namespace BnD;

void B1SECS2ClientSession::recvMessageS64F2(bool wait, const std::vector<uint8>& systemBytes, const uint8* data, uint32 dataLength)
{
    B1SECS2MessageS64F2Readable message(wait, data, dataLength, secs2DataManager());
    if (message.readData() != true) {
        return;
    }
    onRecvMessageS64F2(systemBytes, message.hcAck(), message.cps());
}