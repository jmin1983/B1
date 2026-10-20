//
// B1SECS2MessageS10F2Readable.cpp
//
// Library: B1SECS2
// Package: S10
// Module:  B1SECS2
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1SECS2.h"
#include "B1SECS2MessageS10F2Readable.h"

using namespace BnD;

B1SECS2MessageS10F2Readable::B1SECS2MessageS10F2Readable(const uint8* data, uint32 dataLength, const B1SECS2DataManager* dataManager)
    : B1SECS2MessageReadable(false, 10, 2, "recvMessageS10F2", data, dataLength, dataManager)
    , _ackc10(dataManager)
{
}

B1SECS2MessageS10F2Readable::~B1SECS2MessageS10F2Readable()
{
}

B1String B1SECS2MessageS10F2Readable::fullName()
{
    return "Terminal Request Acknowledge (TRA)";
}

void B1SECS2MessageS10F2Readable::implToString(B1String* string)
{
    appendStringDepth0(_ackc10->toString(), string);
}

bool B1SECS2MessageS10F2Readable::implReadData()
{
    /* Terminal Request Acknowledge (TRA)
    <ACKC10>
    */
    if (readSECS2Data(&_ackc10) != true) {
        return false;
    }
    return true;
}
