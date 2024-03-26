//
// B1SECS2MessageS2F17Readable.cpp
//
// Library: B1SECS2
// Package: S2
// Module:  B1SECS2
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1SECS2.h"
#include "B1SECS2MessageS2F17Readable.h"

using namespace BnD;

B1SECS2MessageS2F17Readable::B1SECS2MessageS2F17Readable(bool wait, const uint8* data, uint32 dataLength)
    : B1SECS2MessageReadable(wait, 2, 17, "recvMessageS2F17", data, dataLength, NULL)
{
}

B1SECS2MessageS2F17Readable::~B1SECS2MessageS2F17Readable()
{
}

B1String B1SECS2MessageS2F17Readable::fullName()
{
    return "Date and Time Request (DTR)";
}

bool B1SECS2MessageS2F17Readable::implReadData()
{
    /* New Equipment Constant Acknowledge (ECA)
    W
    Header Only
    */
    return true;
}
