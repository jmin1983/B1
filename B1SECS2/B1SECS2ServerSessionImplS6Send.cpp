//
// B1SECS2ServerSessionImplS6Send.cpp
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
#include "B1SECS2MessageS6F11Writable.h"
#include "B1SECS2MessageS6F16Writable.h"
#include "B1SECS2MessageS6F20Writable.h"

using namespace BnD;

void B1SECS2ServerSession::sendMessageS6F11(uint16 sessionID, const std::vector<uint8>& systemBytes,
                                          const B1SECS2DataDATAID& dataID, const B1SECS2DataCEID& ceID, const std::map<B1SECS2DataRPTID, std::list<B1SECS2DataV> >& reportsData)
{
    _t6Checker.reset();
    B1SECS2MessageS6F11Writable messageS6F11(dataID, ceID, reportsData);
    if (messageS6F11.writeData() != true) {
        writeLog("sendMessageS6F0");
        sendMessageS6F0(sessionID, systemBytes);
        return;
    }
    sendDataMessage(sessionID, systemBytes, messageS6F11);
}

void B1SECS2ServerSession::sendMessageS6F16(uint16 sessionID, const std::vector<uint8>& systemBytes,
                                          const B1SECS2DataDATAID& dataID, const B1SECS2DataCEID& ceID, const std::map<B1SECS2DataRPTID, std::list<B1SECS2DataV> >& reportsData)
{
    B1SECS2MessageS6F16Writable messageS6F16(dataID, ceID, reportsData);
    if (messageS6F16.writeData() != true) {
        writeLog("sendMessageS6F0");
        sendMessageS6F0(sessionID, systemBytes);
        return;
    }
    sendDataMessage(sessionID, systemBytes, messageS6F16);
}

void B1SECS2ServerSession::sendMessageS6F20(uint16 sessionID, const std::vector<uint8>& systemBytes, const std::list<B1SECS2DataV>& reportData)
{
    B1SECS2MessageS6F20Writable messageS6F20(reportData);
    if (messageS6F20.writeData() != true) {
        writeLog("sendMessageS6F0");
        sendMessageS6F0(sessionID, systemBytes);
        return;
    }
    sendDataMessage(sessionID, systemBytes, messageS6F20);
}
