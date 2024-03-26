//
// B1SECS2MessageS1F13Readable.cpp
//
// Library: B1SECS2
// Package: S1
// Module:  B1SECS2
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1SECS2.h"
#include "B1SECS2MessageS1F13Readable.h"

using namespace BnD;

B1SECS2MessageS1F13Readable::B1SECS2MessageS1F13Readable(bool wait, const uint8* data, uint32 dataLength, const B1SECS2DataManager* dataManager)
    : B1SECS2MessageReadable(wait, 1, 13, "recvMessageS1F13", data, dataLength, dataManager)
    , _list()
{
}

B1SECS2MessageS1F13Readable::~B1SECS2MessageS1F13Readable()
{
}

B1String B1SECS2MessageS1F13Readable::fullName()
{
    return "Establish Communications Request (CR)";
}

void B1SECS2MessageS1F13Readable::implToString(B1String* string)
{
    appendStringDepth0List(_list.count(), string);
}

bool B1SECS2MessageS1F13Readable::implReadData()
{
    /* Establish Communications Request (CR)
    W
    <L 0>
    */
    if (readSECS2Data(&_list) != true) {
        return false;
    }
    return true;
}
