//
// B1SECS2ServerSessionImplS1Send.cpp
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
#include "B1SECS2MessageS1F2Writable.h"
#include "B1SECS2MessageS1F3Writable.h"
#include "B1SECS2MessageS1F4Writable.h"
#include "B1SECS2MessageS1F12Writable.h"
#include "B1SECS2MessageS1F13Writable.h"
#include "B1SECS2MessageS1F14Writable.h"
#include "B1SECS2MessageS1F16Writable.h"
#include "B1SECS2MessageS1F18Writable.h"
#include "B1SECS2MessageS1F22Writable.h"
#include "B1SECS2MessageS1F24Writable.h"

using namespace BnD;

void B1SECS2ServerSession::sendMessageS1F1(uint16 sessionID, const std::vector<uint8>& systemBytes)
{
    /* Are You There Request(R)
    W
    */
    _t6Checker.reset();
    B1LOG("sendMessageS1F1\nAre You There Request (R)");
    sendDataMessage(sessionID, systemBytes, 1, 1, true);
}

void B1SECS2ServerSession::sendMessageS1F2(uint16 sessionID, const std::vector<uint8>& systemBytes, const B1SECS2DataMDLN& mdln, const B1SECS2DataSOFTREV& softRev)
{
    B1SECS2MessageS1F2Writable messageS1F2(mdln, softRev);
    if (messageS1F2.writeData() != true) {
        B1LOG("sendMessageS1F0");
        sendMessageS1F0(sessionID, systemBytes);
        return;
    }
    sendDataMessage(sessionID, systemBytes, messageS1F2);
}

void B1SECS2ServerSession::sendMessageS1F4(uint16 sessionID, const std::vector<uint8>& systemBytes, const std::list<B1SECS2DataSV>& svs)
{
    B1SECS2MessageS1F4Writable messageS1F4(svs);
    if (messageS1F4.writeData() != true) {
        B1LOG("sendMessageS1F0");
        sendMessageS1F0(sessionID, systemBytes);
        return;
    }
    sendDataMessage(sessionID, systemBytes, messageS1F4);
}

void B1SECS2ServerSession::sendMessageS1F12(uint16 sessionID, const std::vector<uint8>& systemBytes,
                                          const std::map<B1SECS2DataSVID, std::pair<B1SECS2DataSVNAME, B1SECS2DataUNITS> >& svs)
{
    B1SECS2MessageS1F12Writable messageS1F12(svs);
    if (messageS1F12.writeData() != true) {
        B1LOG("sendMessageS1F0");
        sendMessageS1F0(sessionID, systemBytes);
        return;
    }
    sendDataMessage(sessionID, systemBytes, messageS1F12);
}

void B1SECS2ServerSession::sendMessageS1F13(uint16 sessionID, const std::vector<uint8>& systemBytes, const B1SECS2DataMDLN& mdln, const B1SECS2DataSOFTREV& softRev)
{
    B1SECS2MessageS1F13Writable messageS1F13(mdln, softRev);
    if (messageS1F13.writeData() != true) {
        B1LOG("sendMessageS1F0");
        sendMessageS1F0(sessionID, systemBytes);
        return;
    }
    sendDataMessage(sessionID, systemBytes, 1, 13, messageS1F13.items());
}

void B1SECS2ServerSession::sendMessageS1F14(uint16 sessionID, const std::vector<uint8>& systemBytes,
                                          const B1SECS2DataCOMMACK& commAck, const B1SECS2DataMDLN& mdln, const B1SECS2DataSOFTREV& softRev)
{
    B1SECS2MessageS1F14Writable messageS1F14(commAck, &mdln, &softRev);
    if (messageS1F14.writeData() != true) {
        B1LOG("sendMessageS1F0");
        sendMessageS1F0(sessionID, systemBytes);
        return;
    }
    sendDataMessage(sessionID, systemBytes, 1, 14, messageS1F14.items());
}

void B1SECS2ServerSession::sendMessageS1F16(uint16 sessionID, const std::vector<uint8>& systemBytes, const B1SECS2DataOFLACK& oflAck)
{
    B1SECS2MessageS1F16Writable messageS1F16(oflAck);
    if (messageS1F16.writeData() != true) {
        B1LOG("sendMessageS1F0");
        sendMessageS1F0(sessionID, systemBytes);
        return;
    }
    sendDataMessage(sessionID, systemBytes, messageS1F16);
}

void B1SECS2ServerSession::sendMessageS1F18(uint16 sessionID, const std::vector<uint8>& systemBytes, const B1SECS2DataONLACK& onlAck)
{
    B1SECS2MessageS1F18Writable messageS1F18(onlAck);
    if (messageS1F18.writeData() != true) {
        B1LOG("sendMessageS1F0");
        sendMessageS1F0(sessionID, systemBytes);
        return;
    }
    sendDataMessage(sessionID, systemBytes, messageS1F18);
}

void B1SECS2ServerSession::sendMessageS1F22(uint16 sessionID, const std::vector<uint8>& systemBytes,
                                          const std::map<B1SECS2DataVID, std::pair<B1SECS2DataDVVALNAME, B1SECS2DataUNITS> >& vNames)
{
    B1SECS2MessageS1F22Writable messageS1F22(vNames);
    if (messageS1F22.writeData() != true) {
        B1LOG("sendMessageS1F0");
        sendMessageS1F0(sessionID, systemBytes);
        return;
    }
    sendDataMessage(sessionID, systemBytes, messageS1F22);
}

void B1SECS2ServerSession::sendMessageS1F24(uint16 sessionID, const std::vector<uint8>& systemBytes,
                                          const std::map<B1SECS2DataCEID, std::pair<B1SECS2DataCENAME, std::list<B1SECS2DataVID> > >& ceNames)
{
    B1SECS2MessageS1F24Writable messageS1F24(ceNames);
    if (messageS1F24.writeData() != true) {
        B1LOG("sendMessageS1F0");
        sendMessageS1F0(sessionID, systemBytes);
        return;
    }
    sendDataMessage(sessionID, systemBytes, messageS1F24);
}
