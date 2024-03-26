//
// B1SECS2ServerSessionImplS64Send.cpp
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
#include "B1SECS2MessageS64F2Writable.h"

using namespace BnD;

void B1SECS2ServerSession::sendMessageS64F2(uint16 sessionID, const std::vector<uint8>& systemBytes, const B1SECS2DataHCACK& hcAck, const std::map<B1SECS2DataCPNAME, B1SECS2DataCPACK>& cps)
{
    B1SECS2MessageS64F2Writable messageS66F2(hcAck, cps);
    if (messageS66F2.writeData() != true) {
        writeLog("sendMessageS64F0");
        sendMessageS64F0(sessionID, systemBytes);
        return;
    }
    sendDataMessage(sessionID, systemBytes, messageS66F2);
}
