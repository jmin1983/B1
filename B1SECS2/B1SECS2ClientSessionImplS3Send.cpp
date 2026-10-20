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
#include "B1SECS2MessageS3F17Writable.h"

using namespace BnD;

bool B1SECS2ClientSession::sendMessageS3F17(const B1SECS2DataDATAID& dataID,
                                            const B1SECS2DataCARRIERACTION& carrierAction,
                                            const B1SECS2DataCARRIERID& carrierID,
                                            const B1SECS2DataPTN& ptn,
                                            const std::map<B1SECS2DataCATTRID, B1SECS2DataCATTRDATA>& cattrData)
{
    B1SECS2MessageS3F17Writable message(dataID, carrierAction, carrierID, ptn, cattrData);
    if (message.writeData() != true) {
        return false;
    }
    sendDataMessage(makeNewSystemBytes(), message);
    return true;
}
