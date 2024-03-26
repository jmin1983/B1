//
// B1SECS2MessageS2F16Readable.cpp
//
// Library: B1SECS2
// Package: S2
// Module:  B1SECS2
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1SECS2.h"
#include "B1SECS2MessageS2F16Readable.h"

using namespace BnD;

B1SECS2MessageS2F16Readable::B1SECS2MessageS2F16Readable(bool wait, const uint8* data, uint32 dataLength, const B1SECS2DataManager* dataManager)
    : B1SECS2MessageReadable(wait, 2, 16, "recvMessageS2F16", data, dataLength, dataManager)
    , _eac(dataManager)
{
}

B1SECS2MessageS2F16Readable::~B1SECS2MessageS2F16Readable()
{
}

B1String B1SECS2MessageS2F16Readable::fullName()
{
    return "New Equipment Constant Acknowledge (ECA)";
}

void B1SECS2MessageS2F16Readable::implToString(B1String* string)
{
    appendStringDepth0(_eac->toString(),string);
}

bool B1SECS2MessageS2F16Readable::implReadData()
{
    /* New Equipment Constant Acknowledge (ECA)
    W
    <EAC>
    */
    if (readSECS2Data(&_eac) != true) {
        return false;
    }
    return true;
}
