//
// SECS2ClientSessionImplS2Send.cpp
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
#include "B1SECS2MessageS2F13Writable.h"
#include "B1SECS2MessageS2F15Writable.h"
#include "B1SECS2MessageS2F17Writable.h"
#include "B1SECS2MessageS2F29Writable.h"
#include "B1SECS2MessageS2F31Writable.h"
#include "B1SECS2MessageS2F33Writable.h"
#include "B1SECS2MessageS2F35Writable.h"
#include "B1SECS2MessageS2F37Writable.h"
#include "B1SECS2MessageS2F41Writable.h"
#include "B1SECS2MessageS2F49Writable.h"

using namespace BnD;

bool B1SECS2ClientSession::sendMessageS2F13(const std::list<B1SECS2DataECID>& ecIDs)
{
    B1SECS2MessageS2F13Writable message(ecIDs);
    if (message.writeData() != true) {
        return false;
    }
    sendDataMessage(makeNewSystemBytes(), message);
    return true;
}

bool B1SECS2ClientSession::sendMessageS2F15(const std::map<B1SECS2DataECID, B1SECS2DataECV>& ecvs)
{
    B1SECS2MessageS2F15Writable message(ecvs);
    if (message.writeData() != true) {
        return false;
    }
    sendDataMessage(makeNewSystemBytes(), message);
    return true;
}

bool B1SECS2ClientSession::sendMessageS2F17()
{
    B1SECS2MessageS2F17Writable message;
    if (message.writeData() != true) {
        return false;
    }
    sendDataMessage(makeNewSystemBytes(), message);
    return true;
}

bool B1SECS2ClientSession::sendMessageS2F29(const std::list<B1SECS2DataECID>& ecIDs)
{
    B1SECS2MessageS2F29Writable message(ecIDs);
    if (message.writeData() != true) {
        return false;
    }
    sendDataMessage(makeNewSystemBytes(), message);
    return true;
}

bool B1SECS2ClientSession::sendMessageS2F31(const B1SECS2DataTIME& time)
{
    B1SECS2MessageS2F31Writable message(time);
    if (message.writeData() != true) {
        return false;
    }
    sendDataMessage(makeNewSystemBytes(), message);
    return true;
}

bool B1SECS2ClientSession::sendMessageS2F33(const B1SECS2DataDATAID& dataID, const std::map<B1SECS2DataRPTID, std::list<B1SECS2DataVID> >& reportData)
{
    B1SECS2MessageS2F33Writable message(dataID, reportData);
    if (message.writeData() != true) {
        return false;
    }
    sendDataMessage(makeNewSystemBytes(), message);
    return true;
}

bool B1SECS2ClientSession::sendMessageS2F35(const B1SECS2DataDATAID& dataID, const std::map<B1SECS2DataCEID, std::list<B1SECS2DataRPTID> >& linkEventData)
{
    B1SECS2MessageS2F35Writable message(dataID, linkEventData);
    if (message.writeData() != true) {
        return false;
    }
    sendDataMessage(makeNewSystemBytes(), message);
    return true;
}

bool B1SECS2ClientSession::sendMessageS2F37(const B1SECS2DataCEED& ceed, const std::list<B1SECS2DataCEID>& ceIDs)
{
    B1SECS2MessageS2F37Writable message(ceed, ceIDs);
    if (message.writeData() != true) {
        return false;
    }
    sendDataMessage(makeNewSystemBytes(), message);
    return true;
}

bool B1SECS2ClientSession::sendMessageS2F41(const B1SECS2DataRCMD& rcmd, const std::map<B1SECS2DataCPNAME, B1SECS2DataCPVAL>& cps)
{
    B1SECS2MessageS2F41Writable message(rcmd, cps);
    if (message.writeData() != true) {
        return false;
    }
    sendDataMessage(makeNewSystemBytes(), message);
    return true;
}

bool B1SECS2ClientSession::sendMessageS2F49(const B1SECS2DataDATAID& dataID, const B1SECS2DataOBJSPEC& objSpec, const B1SECS2DataRCMD& rcmd,
                                          const std::map<B1SECS2DataCPNAME, B1SECS2DataCEPVAL>& cps)
{
    B1SECS2MessageS2F49Writable message(dataID, objSpec, rcmd, cps);
    if (message.writeData() != true) {
        return false;
    }
    sendDataMessage(makeNewSystemBytes(), message);
    return true;
}
