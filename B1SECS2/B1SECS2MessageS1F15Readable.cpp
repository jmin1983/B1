//
// B1SECS2MessageS1F15Readable.cpp
//
// Library: B1SECS2
// Package: S1
// Module:  B1SECS2
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1SECS2.h"
#include "B1SECS2MessageS1F15Readable.h"

using namespace BnD;

B1SECS2MessageS1F15Readable::B1SECS2MessageS1F15Readable(bool wait, const uint8* data, uint32 dataLength)
    : B1SECS2MessageReadable(wait, 1, 15, "recvMessageS1F15", data, dataLength, NULL)
{
}

B1SECS2MessageS1F15Readable::~B1SECS2MessageS1F15Readable()
{
}

B1String B1SECS2MessageS1F15Readable::fullName()
{
    return "Request OFF-LINE (ROFL)";
}

bool B1SECS2MessageS1F15Readable::implReadData()
{
    /* Request OFF - LINE (ROFL)
        Header Only
    */
    return true;
}
