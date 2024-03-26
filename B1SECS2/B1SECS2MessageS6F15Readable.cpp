//
// B1SECS2MessageS6F15Readable.cpp
//
// Library: B1SECS2
// Package: S6
// Module:  B1SECS2
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1SECS2.h"
#include "B1SECS2MessageS6F15Readable.h"

using namespace BnD;

B1SECS2MessageS6F15Readable::B1SECS2MessageS6F15Readable(bool wait, const uint8* data, uint32 dataLength, const B1SECS2DataManager* dataManager)
    : B1SECS2MessageReadable(wait, 6, 15, "recvMessageS6F15", data, dataLength, dataManager)
    , _ceID(dataManager)
{
}

B1SECS2MessageS6F15Readable::~B1SECS2MessageS6F15Readable()
{
}

B1String B1SECS2MessageS6F15Readable::fullName()
{
    return "Event Report Request (ERR)";
}

void B1SECS2MessageS6F15Readable::implToString(B1String* string)
{
    appendStringDepth0(_ceID->toString(), string);
}

bool B1SECS2MessageS6F15Readable::implReadData()
{
    /* Event Report Request (ERR)
    W
    <CEID>
    */
    if (readSECS2Data(&_ceID) != true) {
        return false;
    }
    return true;
}
