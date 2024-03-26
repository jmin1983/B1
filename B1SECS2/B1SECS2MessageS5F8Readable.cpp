//
// B1SECS2MessageS5F8Readable.cpp
//
// Library: B1SECS2
// Package: S5
// Module:  B1SECS2
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1SECS2.h"
#include "B1SECS2MessageS5F8Readable.h"

using namespace BnD;

B1SECS2MessageS5F8Readable::B1SECS2MessageS5F8Readable(bool wait, const uint8* data, uint32 dataLength, const B1SECS2DataManager* dataManager)
    : B1SECS2MessageReadable(wait, 5, 8, "recvMessageS5F8", data, dataLength, dataManager)
    , _internalData()
{
}

B1SECS2MessageS5F8Readable::~B1SECS2MessageS5F8Readable()
{
}

B1String B1SECS2MessageS5F8Readable::fullName()
{
    return "List Enabled Alarm Daata (LEAD)";
}

void B1SECS2MessageS5F8Readable::implToString(B1String* string)
{
    appendStringDepth0List(_internalData.size(), string);
    for (size_t i = 0; i < _internalData.size(); ++i) {
        appendStringDepth1List(3, string);
        appendStringDepth2(_internalData[i]._alCD->toString(), string);
        appendStringDepth2(_internalData[i]._alID->toString(), string);
        appendStringDepth2(_internalData[i]._alTX->toString(), string);
        appendStringDepth1Closer(string);
    }
    appendStringDepth0Closer(string);
}

bool B1SECS2MessageS5F8Readable::implReadData()
{
    /* List Enabled Alarm Request (LEAR)
    W
    <L n
        <L 3
            <ALCD>
            <ALID>
            <ALTX>
        >
    >
    */
    B1SECS2DataLIST list;
    if (readSECS2Data(&list) != true) {
        return false;
    }
    _internalData.reserve(list.count());
    for (uint32 i = 0; i < list.count(); i++) {
        if (readDummyList(3) != true) {
            return false;
        }
        InternalData internalData(dataManager());
        if (readSECS2Data(&internalData._alCD) != true) {
            return false;
        }
        if (readSECS2Data(&internalData._alID) != true) {
            return false;
        }
        if (readSECS2Data(&internalData._alTX) != true) {
            return false;
        }
        _internalData.push_back(internalData);
    }
    return true;
}
