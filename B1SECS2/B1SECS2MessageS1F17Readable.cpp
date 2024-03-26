//
// B1SECS2MessageS1F17Readable.cpp
//
// Library: B1SECS2
// Package: S1
// Module:  B1SECS2
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1SECS2.h"
#include "B1SECS2MessageS1F17Readable.h"

using namespace BnD;

B1SECS2MessageS1F17Readable::B1SECS2MessageS1F17Readable(bool wait, const uint8* data, uint32 dataLength)
    : B1SECS2MessageReadable(wait, 1, 17, "recvMessageS1F17", data, dataLength, NULL)
{
}

B1SECS2MessageS1F17Readable::~B1SECS2MessageS1F17Readable()
{
}

B1String B1SECS2MessageS1F17Readable::fullName()
{
    return "Request ON-LINE (RONL)";
}

bool B1SECS2MessageS1F17Readable::implReadData()
{
    /* ON-LINE Acknowledge (ONLA)
    * Header Only
    */
    return true;
}
