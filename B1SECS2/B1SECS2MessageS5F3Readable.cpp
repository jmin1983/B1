//
// B1SECS2MessageS5F3Readable.cpp
//
// Library: B1SECS2
// Package: S5
// Module:  B1SECS2
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1SECS2.h"
#include "B1SECS2MessageS5F3Readable.h"

using namespace BnD;

B1SECS2MessageS5F3Readable::B1SECS2MessageS5F3Readable(bool wait, const uint8* data, uint32 dataLength, const B1SECS2DataManager* dataManager)
    : B1SECS2MessageReadable(wait, 5, 3, "recvMessageS5F3", data, dataLength, dataManager)
    , _alED(dataManager)
    , _alID(dataManager)
{
}

B1SECS2MessageS5F3Readable::~B1SECS2MessageS5F3Readable()
{
}

B1String B1SECS2MessageS5F3Readable::fullName()
{
    return "Enable/Disable Alarm Send (EAS)";
}

void B1SECS2MessageS5F3Readable::implToString(B1String* string)
{
    appendStringDepth0List(2, string);
    appendStringDepth1(_alED->toString(), string);
    appendStringDepth1(_alID->toString(), string);
    appendStringDepth0Closer(string);
}

bool B1SECS2MessageS5F3Readable::implReadData()
{
    /* Enable/Disable Alarm Send (EAS)
    W
    <L 2
        <ALED>
        <ALID>
    >
    */
    if (readDummyList(2) != true) {
        return false;
    }
    if (readSECS2Data(&_alED) != true) {
        return false;
    }
    if (readSECS2Data(&_alID) != true) {
        return false;
    }
    return true;
}
