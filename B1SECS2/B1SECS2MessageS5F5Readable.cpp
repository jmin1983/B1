//
// B1SECS2MessageS5F5Readable.cpp
//
// Library: B1SECS2
// Package: S5
// Module:  B1SECS2
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1SECS2.h"
#include "B1SECS2MessageS5F5Readable.h"

using namespace BnD;

B1SECS2MessageS5F5Readable::B1SECS2MessageS5F5Readable(bool wait, const uint8* data, uint32 dataLength, const B1SECS2DataManager* dataManager)
    : B1SECS2MessageReadable(wait, 5, 5, "recvMessageS5F5", data, dataLength, dataManager)
    , _alID(dataManager)
{
}

B1SECS2MessageS5F5Readable::~B1SECS2MessageS5F5Readable()
{
}

B1String B1SECS2MessageS5F5Readable::fullName()
{
    return "List Alarm Request (LAR)";
}

void B1SECS2MessageS5F5Readable::implToString(B1String* string)
{
    appendStringDepth0(_alID->toString(), string);
}

bool B1SECS2MessageS5F5Readable::implReadData()
{
    /* List Alarm Request (LAR)
    W
    <ALID>
    >
    */
    if (readSECS2Data(&_alID) != true) {
        return false;
    }
    return true;
}
