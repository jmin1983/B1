//
// B1SECS2MessageS10F4Readable.cpp
//
// Library: B1SECS2
// Package: S10
// Module:  B1SECS2
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1SECS2.h"
#include "B1SECS2MessageS10F4Readable.h"

using namespace BnD;

B1SECS2MessageS10F4Readable::B1SECS2MessageS10F4Readable(const uint8* data, uint32 dataLength, const B1SECS2DataManager* dataManager)
    : B1SECS2MessageReadable(false, 10, 4, "recvMessageS10F4", data, dataLength, dataManager)
    , _ackc10(dataManager)
{
}

B1SECS2MessageS10F4Readable::~B1SECS2MessageS10F4Readable()
{
}

B1String B1SECS2MessageS10F4Readable::fullName()
{
    return "Terminal Display, Single Acknowledge (VTA)";
}

void B1SECS2MessageS10F4Readable::implToString(B1String* string)
{
    appendStringDepth0(_ackc10->toString(), string);
}

bool B1SECS2MessageS10F4Readable::implReadData()
{
    /* Terminal Display, Single Acknowledge (VTA)
    <ACKC10>
    */
    if (readSECS2Data(&_ackc10) != true) {
        return false;
    }
    return true;
}
