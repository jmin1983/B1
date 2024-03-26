//
// B1SECS2MessageS5F7Readable.cpp
//
// Library: B1SECS2
// Package: S5
// Module:  B1SECS2
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1SECS2.h"
#include "B1SECS2MessageS5F7Readable.h"

using namespace BnD;

B1SECS2MessageS5F7Readable::B1SECS2MessageS5F7Readable(bool wait, const uint8* data, uint32 dataLength)
    : B1SECS2MessageReadable(wait, 5, 7, "recvMessageS5F7", data, dataLength, NULL)
{
}

B1SECS2MessageS5F7Readable::~B1SECS2MessageS5F7Readable()
{
}

B1String B1SECS2MessageS5F7Readable::fullName()
{
    return "List Enabled Alarm Request (LEAR)";
}

void B1SECS2MessageS5F7Readable::implToString(B1String* string)
{
}

bool B1SECS2MessageS5F7Readable::implReadData()
{
    /* List Enabled Alarm Request (LEAR)
    W
    */
    return true;
}
