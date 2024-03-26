//
// B1SECS2MessageS2F34Readable.cpp
//
// Library: B1SECS2
// Package: S2
// Module:  B1SECS2
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1SECS2.h"
#include "B1SECS2MessageS2F34Readable.h"

using namespace BnD;

B1SECS2MessageS2F34Readable::B1SECS2MessageS2F34Readable(bool wait, const uint8* data, uint32 dataLength, const B1SECS2DataManager* dataManager)
    : B1SECS2MessageReadable(wait, 2, 34, "recvMessageS2F34", data, dataLength, dataManager)
    , _drAck(dataManager)
{
}

B1SECS2MessageS2F34Readable::~B1SECS2MessageS2F34Readable()
{
}

B1String B1SECS2MessageS2F34Readable::fullName()
{
    return "Define Report Acknowledge (DRA)";
}

void B1SECS2MessageS2F34Readable::implToString(B1String* string)
{
    appendStringDepth0(_drAck->toString(), string);
}

bool B1SECS2MessageS2F34Readable::implReadData()
{
    /* Define Report Acknowledge (DRA)
    W
    <DRACK>
    */
    if (readSECS2Data(&_drAck) != true) {
        return false;
    }
    return true;
}
