//
// B1SECS2MessageS6F19Readable.cpp
//
// Library: B1SECS2
// Package: S6
// Module:  B1SECS2
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1SECS2.h"
#include "B1SECS2MessageS6F19Readable.h"

using namespace BnD;

B1SECS2MessageS6F19Readable::B1SECS2MessageS6F19Readable(bool wait, const uint8* data, uint32 dataLength, const B1SECS2DataManager* dataManager)
    : B1SECS2MessageReadable(wait, 6, 19, "recvMessageS6F19", data, dataLength, dataManager)
    , _rptID(dataManager)
{
}

B1SECS2MessageS6F19Readable::~B1SECS2MessageS6F19Readable()
{
}

B1String B1SECS2MessageS6F19Readable::fullName()
{
    return "Individual Report Request (IRR)";
}

void B1SECS2MessageS6F19Readable::implToString(B1String* string)
{
    appendStringDepth0(_rptID->toString(), string);
}

bool B1SECS2MessageS6F19Readable::implReadData()
{
    /* Individual Report Request (IRR)
    W
    <RPTID>
    */
    if (readSECS2Data(&_rptID) != true) {
        return false;
    }
    return true;
}
