//
// B1SECS2MessageS10F6Readable.cpp
//
// Library: B1SECS2
// Package: S10
// Module:  B1SECS2
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1SECS2.h"
#include "B1SECS2MessageS10F6Readable.h"

using namespace BnD;

B1SECS2MessageS10F6Readable::B1SECS2MessageS10F6Readable(const uint8* data, uint32 dataLength, const B1SECS2DataManager* dataManager)
    : B1SECS2MessageReadable(false, 10, 6, "recvMessageS10F6", data, dataLength, dataManager)
    , _ackc10(dataManager)
{
}

B1SECS2MessageS10F6Readable::~B1SECS2MessageS10F6Readable()
{
}

B1String B1SECS2MessageS10F6Readable::fullName()
{
    return "Terminal Display, Multi-Block Acknowledge (VMA)";
}

void B1SECS2MessageS10F6Readable::implToString(B1String* string)
{
    appendStringDepth0(_ackc10->toString(), string);
}

bool B1SECS2MessageS10F6Readable::implReadData()
{
    /* Terminal Display, Multi-Block Acknowledge (VMA)
    <ACKC10>
    */
    if (readSECS2Data(&_ackc10) != true) {
        return false;
    }
    return true;
}
