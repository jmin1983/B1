//
// B1SECS2MessageS5F1Readable.cpp
//
// Library: B1SECS2
// Package: S5
// Module:  B1SECS2
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1SECS2.h"
#include "B1SECS2MessageS5F1Readable.h"

using namespace BnD;

B1SECS2MessageS5F1Readable::B1SECS2MessageS5F1Readable(bool wait, const uint8* data, uint32 dataLength, const B1SECS2DataManager* dataManager)
    : B1SECS2MessageReadable(wait, 5, 1, "recvMessageS5F1", data, dataLength, dataManager)
    , _alCD(dataManager)
    , _alID(dataManager)
    , _alTX(dataManager)
{
}

B1SECS2MessageS5F1Readable::~B1SECS2MessageS5F1Readable()
{
}

B1String B1SECS2MessageS5F1Readable::fullName()
{
    return "Alarm Report Send (ARS)";
}

void B1SECS2MessageS5F1Readable::implToString(B1String* string)
{
    appendStringDepth0List(3, string);
    appendStringDepth1(_alCD->toString(), string);
    appendStringDepth1(_alID->toString(), string);
    appendStringDepth1(_alTX->toString(), string);
    appendStringDepth0Closer(string);
}

bool B1SECS2MessageS5F1Readable::implReadData()
{
    /*  Alarm Report Send (ARS)
    <L 3
        <ALCD>
        <ALID>
        <ALTX>
    >
    */
    if (readDummyList(3) != true) {
        return false;
    }
    if (readSECS2Data(&_alCD) != true) {
        return false;
    }
    if (readSECS2Data(&_alID) != true) {
        return false;
    }
    if (readSECS2Data(&_alTX) != true) {
        return false;
    }
    return true;
}
