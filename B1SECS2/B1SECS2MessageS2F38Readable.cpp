//
// B1SECS2MessageS2F38Readable.cpp
//
// Library: B1SECS2
// Package: S2
// Module:  B1SECS2
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1SECS2.h"
#include "B1SECS2MessageS2F38Readable.h"

using namespace BnD;

B1SECS2MessageS2F38Readable::B1SECS2MessageS2F38Readable(bool wait, const uint8* data, uint32 dataLength, const B1SECS2DataManager* dataManager)
    : B1SECS2MessageReadable(wait, 2, 38, "recvMessageS2F38", data, dataLength, dataManager)
    , _erAck(dataManager)
{
}

B1SECS2MessageS2F38Readable::~B1SECS2MessageS2F38Readable()
{
}

B1String B1SECS2MessageS2F38Readable::fullName()
{
    return "Enable/Disable Event Report Acknowledge (EERA)";
}

void B1SECS2MessageS2F38Readable::implToString(B1String* string)
{
    appendStringDepth1(_erAck->toString(), string);
}

bool B1SECS2MessageS2F38Readable::implReadData()
{
    /* Enable/Disable Event Report Acknowledge (EERA)
    W
    <ERACK>
    */
    if (readSECS2Data(&_erAck) != true) {
        return false;
    }
    return true;
}
