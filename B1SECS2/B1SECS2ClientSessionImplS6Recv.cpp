//
// SECS2ClientSessionImplS6Recv.cpp
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
#include "B1SECS2MessageS6F11Readable.h"

using namespace BnD;

void B1SECS2ClientSession::recvMessageS6F11(bool wait, const std::vector<uint8>& systemBytes, const uint8* data, uint32 dataLength)
{
    B1SECS2MessageS6F11Readable message(wait, data, dataLength, secs2DataManager());
    if (message.readData() != true) {
        return;
    }
    onRecvMessageS6F11(wait, systemBytes, message.dataID(), message.ceID(), message.reportsData());
}
