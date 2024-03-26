//
// B1SECS2MessageS1F12Readable.cpp
//
// Library: B1SECS2
// Package: S1
// Module:  B1SECS2
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1SECS2.h"
#include "B1SECS2MessageS1F12Readable.h"

using namespace BnD;

B1SECS2MessageS1F12Readable::B1SECS2MessageS1F12Readable(bool wait, const uint8* data, uint32 dataLength, const B1SECS2DataManager* dataManager)
    : B1SECS2MessageReadable(wait, 1, 12, "recvMessageS1F12", data, dataLength, dataManager)
    , _internalData()
{
}

B1SECS2MessageS1F12Readable::~B1SECS2MessageS1F12Readable()
{
}

B1String B1SECS2MessageS1F12Readable::fullName()
{
    return "Status Varible NameList Reply (SVNRR)";
}

void B1SECS2MessageS1F12Readable::implToString(B1String* string)
{
    appendStringDepth0List(_internalData.size(), string);
    for (size_t i = 0; i < _internalData.size(); ++i) {
        appendStringDepth1List(3, string);
        appendStringDepth2(_internalData[i]._svID->toString(), string);
        appendStringDepth2(_internalData[i]._svName->toString(), string);
        appendStringDepth2(_internalData[i]._units->toString(), string);
        appendStringDepth1Closer(string);
    }
    appendStringDepth0Closer(string);
}

bool B1SECS2MessageS1F12Readable::implReadData()
{
    /* Establish Communications Request (CR)
    <L a
        <L 3
            <SVID>
            <SVNAME>
            <UNITS>
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
        if (readSECS2Data(&internalData._svID) != true) {
            return false;
        }
        if (readSECS2Data(&internalData._svName) != true) {
            return false;
        }
        if (readSECS2Data(&internalData._units) != true) {
            return false;
        }
        _internalData.push_back(internalData);
    }
    return true;
}
