//
// B1SECS2MessageS2F36Readable.cpp
//
// Library: B1SECS2
// Package: S2
// Module:  B1SECS2
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1SECS2.h"
#include "B1SECS2MessageS2F36Readable.h"

using namespace BnD;

B1SECS2MessageS2F36Readable::B1SECS2MessageS2F36Readable(bool wait, const uint8* data, uint32 dataLength, const B1SECS2DataManager* dataManager)
    : B1SECS2MessageReadable(wait, 2, 36, "recvMessageS2F36", data, dataLength, dataManager)
    , _lrAck(dataManager)
{
}

B1SECS2MessageS2F36Readable::~B1SECS2MessageS2F36Readable()
{
}

B1String B1SECS2MessageS2F36Readable::fullName()
{
    return "Link Event Report Acknowledge (LERA)";
}

void B1SECS2MessageS2F36Readable::implToString(B1String* string)
{
    appendStringDepth0(_lrAck->toString(), string);
}

bool B1SECS2MessageS2F36Readable::implReadData()
{
    /* Link Event Report Acknowledge (LERA)
    W
    <LRACK>
    */
    if (readSECS2Data(&_lrAck) != true) {
        return false;
    }
    return true;
}
