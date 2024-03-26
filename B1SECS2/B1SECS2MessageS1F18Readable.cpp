//
// B1SECS2MessageS1F18Readable.cpp
//
// Library: B1SECS2
// Package: S1
// Module:  B1SECS2
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1SECS2.h"
#include "B1SECS2MessageS1F18Readable.h"

using namespace BnD;

B1SECS2MessageS1F18Readable::B1SECS2MessageS1F18Readable(bool wait, const uint8* data, uint32 dataLength, const B1SECS2DataManager* dataManager)
    : B1SECS2MessageReadable(wait, 1, 18, "recvMessageS1F18", data, dataLength, dataManager)
    , _onlAck(dataManager)
{
}

B1SECS2MessageS1F18Readable::~B1SECS2MessageS1F18Readable()
{
}

B1String B1SECS2MessageS1F18Readable::fullName()
{
    return "ON-LINE Acknowledge (ONLA)";
}

void B1SECS2MessageS1F18Readable::implToString(B1String* string)
{
    appendStringDepth0(_onlAck->toString(), string);
}

bool B1SECS2MessageS1F18Readable::implReadData()
{
    /* ON-LINE Acknowledge (ONLA)
    <ONLACK>
    */
    if (readSECS2Data(&_onlAck) != true) {
        return false;
    }
    return true;
}
