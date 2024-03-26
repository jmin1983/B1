//
// B1SECS2MessageS1F16Readable.cpp
//
// Library: B1SECS2
// Package: S1
// Module:  B1SECS2
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1SECS2.h"
#include "B1SECS2MessageS1F16Readable.h"

using namespace BnD;

B1SECS2MessageS1F16Readable::B1SECS2MessageS1F16Readable(bool wait, const uint8* data, uint32 dataLength, const B1SECS2DataManager* dataManager)
    : B1SECS2MessageReadable(wait, 1, 16, "recvMessageS1F16", data, dataLength, dataManager)
    , _oflAck(dataManager)
{
}

B1SECS2MessageS1F16Readable::~B1SECS2MessageS1F16Readable()
{
}

B1String B1SECS2MessageS1F16Readable::fullName()
{
    return "OFF-LINE Acknowledge (OFLA)";
}

void B1SECS2MessageS1F16Readable::implToString(B1String* string)
{
    appendStringDepth0(_oflAck->toString(), string);
}

bool B1SECS2MessageS1F16Readable::implReadData()
{
    /* OFF-LINE Acknowledge (OFLA)
        <OFLACK>
    */
    if (readSECS2Data(&_oflAck) != true) {
        return false;
    }
    return true;
}
