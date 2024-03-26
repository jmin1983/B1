//
// B1SECS2MessageS2F18Readable.cpp
//
// Library: B1SECS2
// Package: S2
// Module:  B1SECS2
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1SECS2.h"
#include "B1SECS2MessageS2F18Readable.h"

using namespace BnD;

B1SECS2MessageS2F18Readable::B1SECS2MessageS2F18Readable(bool wait, const uint8* data, uint32 dataLength, const B1SECS2DataManager* dataManager)
    : B1SECS2MessageReadable(wait, 2, 18, "recvMessageS2F18", data, dataLength, dataManager)
    , _time(dataManager)
{
}

B1SECS2MessageS2F18Readable::~B1SECS2MessageS2F18Readable()
{
}

B1String B1SECS2MessageS2F18Readable::fullName()
{
    return "Date and Time Data (DTD)";
}

void B1SECS2MessageS2F18Readable::implToString(B1String* string)
{
    appendStringDepth0(_time->toString(), string);
}

bool B1SECS2MessageS2F18Readable::implReadData()
{
    /* New Equipment Constant Acknowledge (ECA)
    W
    <TIME>
    */
    if (readSECS2Data(&_time) != true) {
        return false;
    }
    return true;
}
