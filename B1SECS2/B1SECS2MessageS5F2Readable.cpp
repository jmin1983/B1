//
// B1SECS2MessageS5F2Readable.cpp
//
// Library: B1SECS2
// Package: S5
// Module:  B1SECS2
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1SECS2.h"
#include "B1SECS2MessageS5F2Readable.h"

using namespace BnD;

B1SECS2MessageS5F2Readable::B1SECS2MessageS5F2Readable(bool wait, const uint8* data, uint32 dataLength, const B1SECS2DataManager* dataManager)
    : B1SECS2MessageReadable(wait, 5, 2, "recvMessageS5F2", data, dataLength, dataManager)
    , _ackc5(dataManager)
{
}

B1SECS2MessageS5F2Readable::~B1SECS2MessageS5F2Readable()
{
}

B1String B1SECS2MessageS5F2Readable::fullName()
{
    return "Alarm Report Acknowledge (ARA)";
}

void B1SECS2MessageS5F2Readable::implToString(B1String* string)
{
    appendStringDepth0(_ackc5->toString(), string);
}

bool B1SECS2MessageS5F2Readable::implReadData()
{
    /*  Alarm Report Acknowledge (ARA)
    <ACKC5>
    */
    if (readSECS2Data(&_ackc5) != true) {
        return false;
    }
    return true;
}
