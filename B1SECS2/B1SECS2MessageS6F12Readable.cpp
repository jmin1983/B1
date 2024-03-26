//
// B1SECS2MessageS6F12Readable.cpp
//
// Library: B1SECS2
// Package: S6
// Module:  B1SECS2
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1SECS2.h"
#include "B1SECS2MessageS6F12Readable.h"

using namespace BnD;

B1SECS2MessageS6F12Readable::B1SECS2MessageS6F12Readable(bool wait, const uint8* data, uint32 dataLength, const B1SECS2DataManager* dataManager)
    : B1SECS2MessageReadable(wait, 6, 12, "recvMessageS6F12", data, dataLength, dataManager)
    , _ackc6(dataManager)
{
}

B1SECS2MessageS6F12Readable::~B1SECS2MessageS6F12Readable()
{
}

B1String B1SECS2MessageS6F12Readable::fullName()
{
    return "Event Report Acknowledge (ERA)";
}

void B1SECS2MessageS6F12Readable::implToString(B1String* string)
{
    appendStringDepth0(_ackc6->toString(), string);
}

bool B1SECS2MessageS6F12Readable::implReadData()
{
    /* Event Report Acknowledge (ERA)
    <ACKC6>
    */
    if (readSECS2Data(&_ackc6) != true) {
        return false;
    }
    return true;
}
