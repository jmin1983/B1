//
// B1SECS2Client.cpp
//
// Library: B1SECS2
// Package: Client
// Module:  B1SECS2
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1SECS2.h"
#include "B1SECS2Client.h"
#include "B1SECS2ClientSession.h"

#include <B1Network/B1BaseSessionManager.h>

using namespace BnD;

B1SECS2Client::B1SECS2Client(const B1SECS2DataManager* secs2DataManager)
    : B1BaseClient()
    , _secs2DataManager(secs2DataManager)
{
}

B1BaseClientSession* B1SECS2Client::createSession(B1ClientSocket* clientSocket, void* param)
{
    return new B1SECS2ClientSession(clientSocket, this, *(uint16*)param, _secs2DataManager);
}

std::shared_ptr<B1SECS2ClientSession> B1SECS2Client::getSession(uint16 deviceID)
{
    return sessionManager()->getSessionByHandleID<B1SECS2ClientSession>(deviceID);
}

bool B1SECS2Client::initialize()
{
    return startup();
}

void B1SECS2Client::finalize()
{
    shutdown();
}

bool B1SECS2Client::connect(const B1String& address, uint16 port, uint16 deviceID)
{
    B1LOG("connecting: address[%s], port[%u], deviceID[%d]", address.cString(), port, deviceID);
    if (NULL == B1BaseClient::connect(address.copy(), port, &deviceID, deviceID)) {
        B1LOG("connect failed: address[%s], port[%u], deviceID[%d]", address.cString(), port, deviceID);
        return false;
    }
    return true;
}

void B1SECS2Client::disconnect(uint16 deviceID)
{
    disconnectByHandleID(deviceID);
}

bool B1SECS2Client::sendMessageS1F3(uint16 deviceID, const std::list<B1SECS2DataSVID>& svIDs)
{
    if (auto session = getSession(deviceID)) {
        return session->sendMessageS1F3(svIDs);
    }
    return false;
}

bool B1SECS2Client::sendMessageS1F13(uint16 deviceID)
{
    if (auto session = getSession(deviceID)) {
        return session->sendMessageS1F13();
    }
    return false;
}

bool B1SECS2Client::sendMessageS1F15(uint16 deviceID)
{
    if (auto session = getSession(deviceID)) {
        return session->sendMessageS1F15();
    }
    return false;
}

bool B1SECS2Client::sendMessageS1F17(uint16 deviceID)
{
    if (auto session = getSession(deviceID)) {
        return session->sendMessageS1F17();
    }
    return false;
}

bool B1SECS2Client::sendMessageS2F31(uint16 deviceID, const B1SECS2DataTIME& time)
{
    if (auto session = getSession(deviceID)) {
        return session->sendMessageS2F31(time);
    }
    return false;
}

bool B1SECS2Client::sendMessageS2F33(uint16 deviceID, const B1SECS2DataDATAID& dataID, const std::map<B1SECS2DataRPTID, std::list<B1SECS2DataVID> >& reportData)
{
    if (auto session = getSession(deviceID)) {
        return session->sendMessageS2F33(dataID, reportData);
    }
    return false;
}

bool B1SECS2Client::sendMessageS2F35(uint16 deviceID, const B1SECS2DataDATAID& dataID, const std::map<B1SECS2DataCEID, std::list<B1SECS2DataRPTID> >& linkEventData)
{
    if (auto session = getSession(deviceID)) {
        return session->sendMessageS2F35(dataID, linkEventData);
    }
    return false;
}

bool B1SECS2Client::sendMessageS2F37(uint16 deviceID, const B1SECS2DataCEED& ceed, const std::list<B1SECS2DataCEID>& ceIDs)
{
    if (auto session = getSession(deviceID)) {
        return session->sendMessageS2F37(ceed, ceIDs);
    }
    return false;
}

bool B1SECS2Client::sendMessageS2F41(uint16 deviceID, const B1SECS2DataRCMD& rcmd, const std::map<B1SECS2DataCPNAME, B1SECS2DataCPVAL>& cps)
{
    if (auto session = getSession(deviceID)) {
        return session->sendMessageS2F41(rcmd, cps);
    }
    return false;
}

bool B1SECS2Client::sendMessageS2F49(uint16 deviceID,
                                     const B1SECS2DataDATAID& dataID, const B1SECS2DataOBJSPEC& objSpec, const B1SECS2DataRCMD& rcmd,
                                     const std::map<B1SECS2DataCPNAME, B1SECS2DataCEPVAL>& cps)
{
    if (auto session = getSession(deviceID)) {
        return session->sendMessageS2F49(dataID, objSpec, rcmd, cps);
    }
    return false;
}

bool B1SECS2Client::sendMessageS5F3(uint16 deviceID, const B1SECS2DataALED& alED, const B1SECS2DataALID& alID)
{
    if (auto session = getSession(deviceID)) {
        return session->sendMessageS5F3(alED, alID);
    }
    return false;
}

bool B1SECS2Client::sendMessageS64F1(uint16 deviceID, const B1SECS2DataRCMD& rcmd, const std::map<B1SECS2DataCPNAME, B1SECS2DataCPVAL>& cps)
{
    if (auto session = getSession(deviceID)) {
        return session->sendMessageS64F1(rcmd, cps);
    }
    return false;
}
