//
// B1SECS2ServerSessionImplS2Send.cpp
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
#include "B1SECS2MessageS2F14Writable.h"
#include "B1SECS2MessageS2F16Writable.h"
#include "B1SECS2MessageS2F18Writable.h"
#include "B1SECS2MessageS2F30Writable.h"
#include "B1SECS2MessageS2F32Writable.h"
#include "B1SECS2MessageS2F34Writable.h"
#include "B1SECS2MessageS2F36Writable.h"
#include "B1SECS2MessageS2F38Writable.h"
#include "B1SECS2MessageS2F42Writable.h"
#include "B1SECS2MessageS2F50Writable.h"

using namespace BnD;

void B1SECS2ServerSession::sendMessageS2F14(uint16 sessionID, const std::vector<uint8>& systemBytes, const std::list<B1SECS2DataECV>& data)
{
    B1SECS2MessageS2F14Writable messageS2F14(data);
    if (messageS2F14.writeData() != true) {
        writeLog("sendMessageS2F0");
        sendMessageS2F0(sessionID, systemBytes);
        return;
    }
    sendDataMessage(sessionID, systemBytes, messageS2F14);
}

void B1SECS2ServerSession::sendMessageS2F16(uint16 sessionID, const std::vector<uint8>& systemBytes, const B1SECS2DataEAC& eac)
{
    B1SECS2MessageS2F16Writable messageS2F16(eac);
    if (messageS2F16.writeData() != true) {
        writeLog("sendMessageS2F0");
        sendMessageS2F0(sessionID, systemBytes);
        return;
    }
    sendDataMessage(sessionID, systemBytes, messageS2F16);
}

void B1SECS2ServerSession::sendMessageS2F18(uint16 sessionID, const std::vector<uint8>& systemBytes, const B1SECS2DataTIME& time)
{
    B1SECS2MessageS2F18Writable messageS2F18(time);
    if (messageS2F18.writeData() != true) {
        writeLog("sendMessageS2F0");
        sendMessageS2F0(sessionID, systemBytes);
        return;
    }
    sendDataMessage(sessionID, systemBytes, messageS2F18);
}

void B1SECS2ServerSession::sendMessageS2F30(uint16 sessionID, const std::vector<uint8>& systemBytes,
                                          const std::vector<B1SECS2DataECID>& ecIDs, const std::vector<B1SECS2DataECNAME>& ecNames, const std::vector<B1SECS2DataECMIN>& ecMins,
                                          const std::vector<B1SECS2DataECMAX>& ecMaxes, const std::vector<B1SECS2DataECDEF>& ecDefs, const std::vector<B1SECS2DataUNITS>& units)
{
    B1SECS2MessageS2F30Writable messageS2F30(ecIDs, ecNames, ecMins, ecMaxes, ecDefs, units);
    if (messageS2F30.writeData() != true) {
        writeLog("sendMessageS2F0");
        sendMessageS2F0(sessionID, systemBytes);
        return;
    }
    sendDataMessage(sessionID, systemBytes, messageS2F30);
}

void B1SECS2ServerSession::sendMessageS2F32(uint16 sessionID, const std::vector<uint8>& systemBytes, const B1SECS2DataTIACK& tiAck)
{
    B1SECS2MessageS2F32Writable messageS2F32(tiAck);
    if (messageS2F32.writeData() != true) {
        writeLog("sendMessageS2F0");
        sendMessageS2F0(sessionID, systemBytes);
        return;
    }
    sendDataMessage(sessionID, systemBytes, messageS2F32);
}

void B1SECS2ServerSession::sendMessageS2F34(uint16 sessionID, const std::vector<uint8>& systemBytes, const B1SECS2DataDRACK& drAck)
{
    B1SECS2MessageS2F34Writable messageS2F34(drAck);
    if (messageS2F34.writeData() != true) {
        writeLog("sendMessageS2F0");
        sendMessageS2F0(sessionID, systemBytes);
        return;
    }
    sendDataMessage(sessionID, systemBytes, messageS2F34);
}

void B1SECS2ServerSession::sendMessageS2F36(uint16 sessionID, const std::vector<uint8>& systemBytes, const B1SECS2DataLRACK& lrAck)
{
    B1SECS2MessageS2F36Writable messageS2F36(lrAck);
    if (messageS2F36.writeData() != true) {
        writeLog("sendMessageS2F0");
        sendMessageS2F0(sessionID, systemBytes);
        return;
    }
    sendDataMessage(sessionID, systemBytes, messageS2F36);
}

void B1SECS2ServerSession::sendMessageS2F38(uint16 sessionID, const std::vector<uint8>& systemBytes, const B1SECS2DataERACK& erAck)
{
    B1SECS2MessageS2F38Writable messageS2F38(erAck);
    if (messageS2F38.writeData() != true) {
        writeLog("sendMessageS2F0");
        sendMessageS2F0(sessionID, systemBytes);
        return;
    }
    sendDataMessage(sessionID, systemBytes, messageS2F38);
}

void B1SECS2ServerSession::sendMessageS2F42(uint16 sessionID, const std::vector<uint8>& systemBytes, const B1SECS2DataHCACK& hcAck, const std::map<B1SECS2DataCPNAME, B1SECS2DataCPACK>& cps)
{
    B1SECS2MessageS2F42Writable messageS2F42(hcAck, cps);
    if (messageS2F42.writeData() != true) {
        writeLog("sendMessageS2F0");
        sendMessageS2F0(sessionID, systemBytes);
        return;
    }
    sendDataMessage(sessionID, systemBytes, messageS2F42);
}

void B1SECS2ServerSession::sendMessageS2F50(uint16 sessionID, const std::vector<uint8>& systemBytes, const B1SECS2DataHCACK& hcAck, const std::map<B1SECS2DataCPNAME, B1SECS2DataCEPACK>& cps)
{
    B1SECS2MessageS2F50Writable messageS2F50(hcAck, cps);
    if (messageS2F50.writeData() != true) {
        writeLog("sendMessageS2F0");
        sendMessageS2F0(sessionID, systemBytes);
        return;
    }
    sendDataMessage(sessionID, systemBytes, messageS2F50);
}
