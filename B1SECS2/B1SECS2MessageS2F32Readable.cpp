//
// B1SECS2MessageS2F32Readable.cpp
//
// Library: B1SECS2
// Package: S2
// Module:  B1SECS2
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1SECS2.h"
#include "B1SECS2MessageS2F32Readable.h"

using namespace BnD;

B1SECS2MessageS2F32Readable::B1SECS2MessageS2F32Readable(bool wait, const uint8* data, uint32 dataLength, const B1SECS2DataManager* dataManager)
    : B1SECS2MessageReadable(wait, 2, 32, "recvMessageS2F32", data, dataLength, dataManager)
    , _tiAck(dataManager)
{
}

B1SECS2MessageS2F32Readable::~B1SECS2MessageS2F32Readable()
{
}

B1String B1SECS2MessageS2F32Readable::fullName()
{
    return "Date and Time Set Acknowledge (DTA)";
}

void B1SECS2MessageS2F32Readable::implToString(B1String* string)
{
    appendStringDepth1(_tiAck->toString(), string);
}

bool B1SECS2MessageS2F32Readable::implReadData()
{
    /* Date and Time Set Acknowledge (DTA)
    W
    <TIACK>
    */
    if (readSECS2Data(&_tiAck) != true) {
        return false;
    }
    return true;
}
