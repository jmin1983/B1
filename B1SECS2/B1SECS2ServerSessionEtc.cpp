//
// B1SECS2ServerSessionEtc.cpp
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

using namespace BnD;

void B1SECS2ServerSession::onRecvMessageS1F1(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes)
{
    assert(false);
    sendMessageS1F0(sessionID, systemBytes);
}

void B1SECS2ServerSession::onRecvMessageS1F2(uint16 sessionID, const std::vector<uint8>& systemBytes)
{
    assert(false);
    sendMessageS1F0(sessionID, systemBytes);
}

void B1SECS2ServerSession::onRecvMessageS1F3(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes, const std::vector<B1SECS2DataSVID>& svIDs)
{
    assert(false);
    sendMessageS1F0(sessionID, systemBytes);
}

void B1SECS2ServerSession::onRecvMessageS1F11(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes, const std::vector<B1SECS2DataSVID>& svIDs)
{
    assert(false);
    sendMessageS1F0(sessionID, systemBytes);
}

void B1SECS2ServerSession::onRecvMessageS1F13(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes)
{
    assert(false);
    sendMessageS1F0(sessionID, systemBytes);
}

void B1SECS2ServerSession::onRecvMessageS1F14(uint16 sessionID, const std::vector<uint8>& systemBytes, const B1SECS2DataCOMMACK& commack)
{
    assert(false);
    sendMessageS1F0(sessionID, systemBytes);
}

void B1SECS2ServerSession::onRecvMessageS1F15(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes)
{
    assert(false);
    sendMessageS1F0(sessionID, systemBytes);
}

void B1SECS2ServerSession::onRecvMessageS1F17(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes)
{
    assert(false);
    sendMessageS1F0(sessionID, systemBytes);
}

void B1SECS2ServerSession::onRecvMessageS1F21(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes, const std::vector<B1SECS2DataVID>& vIDs)
{
    assert(false);
    sendMessageS1F0(sessionID, systemBytes);
}

void B1SECS2ServerSession::onRecvMessageS1F23(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes, const std::vector<B1SECS2DataCEID>& ceIDs)
{
    assert(false);
    sendMessageS1F0(sessionID, systemBytes);
}

void B1SECS2ServerSession::onRecvMessageS2F13(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes, const std::vector<B1SECS2DataECID>& ecIDs)
{
    assert(false);
    sendMessageS2F0(sessionID, systemBytes);
}

void B1SECS2ServerSession::onRecvMessageS2F15(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes, const std::map<B1SECS2DataECID, B1SECS2DataECV>& ecvs)
{
    assert(false);
    sendMessageS2F0(sessionID, systemBytes);
}

void B1SECS2ServerSession::onRecvMessageS2F17(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes)
{
    assert(false);
    sendMessageS2F0(sessionID, systemBytes);
}

void B1SECS2ServerSession::onRecvMessageS2F29(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes, const std::vector<B1SECS2DataECID>& ecIDs)
{
    assert(false);
    sendMessageS2F0(sessionID, systemBytes);
}

void B1SECS2ServerSession::onRecvMessageS2F31(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes, const B1SECS2DataTIME& time)
{
    assert(false);
    sendMessageS2F0(sessionID, systemBytes);
}

void B1SECS2ServerSession::onRecvMessageS2F33(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes,
                                            const B1SECS2DataDATAID& dataID, const std::map<B1SECS2DataRPTID, std::vector<B1SECS2DataVID> >& reportData)
{
    assert(false);
    sendMessageS2F0(sessionID, systemBytes);
}

void B1SECS2ServerSession::onRecvMessageS2F35(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes,
                                            const B1SECS2DataDATAID& dataID, const std::map<B1SECS2DataCEID, std::vector<B1SECS2DataRPTID> >& linkEventData)
{
    assert(false);
    sendMessageS2F0(sessionID, systemBytes);
}

void B1SECS2ServerSession::onRecvMessageS2F37(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes,
                                            const B1SECS2DataCEED& ceed, const std::vector<B1SECS2DataCEID>& ceIDs)
{
    assert(false);
    sendMessageS2F0(sessionID, systemBytes);
}

void B1SECS2ServerSession::onRecvMessageS2F41(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes,
                                            const B1SECS2DataRCMD& rcmd, const std::map<B1SECS2DataCPNAME, B1SECS2DataCPVAL>& cps)
{
    assert(false);
    sendMessageS2F0(sessionID, systemBytes);
}

void B1SECS2ServerSession::onRecvMessageS2F49(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes,
                                            const B1SECS2DataDATAID& dataID, const B1SECS2DataRCMD& rcmd, const std::map<B1SECS2DataCPNAME, B1SECS2DataCEPVAL>& cps)
{
    assert(false);
    sendMessageS2F0(sessionID, systemBytes);
}

void B1SECS2ServerSession::onRecvMessageS5F2(uint16 sessionID, const std::vector<uint8>& systemBytes, const B1SECS2DataACKC5& ackc5)
{
    assert(false);
    sendMessageS5F0(sessionID, systemBytes);
}

void B1SECS2ServerSession::onRecvMessageS5F3(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes, const B1SECS2DataALED& alED, const B1SECS2DataALID& alID)
{
    assert(false);
    sendMessageS5F0(sessionID, systemBytes);
}

void B1SECS2ServerSession::onRecvMessageS5F5(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes, const B1SECS2DataALID& alID)
{
    assert(false);
    sendMessageS5F0(sessionID, systemBytes);
}

void B1SECS2ServerSession::onRecvMessageS5F7(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes)
{
    assert(false);
    sendMessageS5F0(sessionID, systemBytes);
}

void B1SECS2ServerSession::onRecvMessageS6F12(uint16 sessionID, const std::vector<uint8>& systemBytes, const B1SECS2DataACKC6& ackc6)
{
    assert(false);
    sendMessageS6F0(sessionID, systemBytes);
}

void B1SECS2ServerSession::onRecvMessageS6F15(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes, const B1SECS2DataCEID& ceID)
{
    assert(false);
    sendMessageS6F0(sessionID, systemBytes);
}

void B1SECS2ServerSession::onRecvMessageS6F19(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes, const B1SECS2DataRPTID& rptID)
{
    assert(false);
    sendMessageS6F0(sessionID, systemBytes);
}

void B1SECS2ServerSession::onRecvMessageS64F1(uint16 sessionID, bool wait, const std::vector<uint8>& systemBytes,
                                            const B1SECS2DataRCMD& rcmd, const std::map<B1SECS2DataCPNAME, B1SECS2DataCPVAL>& cps)
{
    assert(false);
    sendMessageS64F0(sessionID, systemBytes);
}
