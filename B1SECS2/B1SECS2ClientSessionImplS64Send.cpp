//
// SECS2ClientSessionImplS64Send.cpp
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
#include "B1SECS2MessageS64F1Writable.h"

using namespace BnD;

bool B1SECS2ClientSession::sendMessageS64F1(const B1SECS2DataRCMD& rcmd, const std::map<B1SECS2DataCPNAME, B1SECS2DataCPVAL>& cps)
{
    B1SECS2MessageS64F1Writable message(rcmd, cps);
    if (message.writeData() != true) {
        return false;
    }
    sendDataMessage(_secs2SessionID, makeNewSystemBytes(), message);
    return true;
}
