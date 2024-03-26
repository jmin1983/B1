//
// B1SECS2MessageS1F22Readable.cpp
//
// Library: B1SECS2
// Package: S1
// Module:  B1SECS2
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1SECS2.h"
#include "B1SECS2MessageS1F22Readable.h"

using namespace BnD;

B1SECS2MessageS1F22Readable::B1SECS2MessageS1F22Readable(bool wait, const uint8* data, uint32 dataLength, const B1SECS2DataManager* dataManager)
    : B1SECS2MessageReadable(wait, 1, 22, "recvMessageS1F22", data, dataLength, dataManager)
    , _internalData()
{
}

B1SECS2MessageS1F22Readable::~B1SECS2MessageS1F22Readable()
{
}

B1String B1SECS2MessageS1F22Readable::fullName()
{
    return "Data Variable Namelist (DVN)";
}

void B1SECS2MessageS1F22Readable::implToString(B1String* string)
{
    appendStringDepth0List(_internalData.size(), string);
    for (size_t i = 0; i < _internalData.size(); ++i) {
        appendStringDepth1List(3, string);
        appendStringDepth2(_internalData[i]._vid->toString(), string);
        appendStringDepth2(_internalData[i]._dvvalName->toString(), string);
        appendStringDepth2(_internalData[i]._units->toString(), string);
        appendStringDepth1Closer(string);
    }
    appendStringDepth0Closer(string);
}

bool B1SECS2MessageS1F22Readable::implReadData()
{
    /* Data Variable Namelist (DVN)
    W
    <L n
        <L 3
            <VID>
            <DVVALNAME>
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
        if (readSECS2Data(&internalData._vid) != true) {
            return false;
        }
        if (readSECS2Data(&internalData._dvvalName) != true) {
            return false;
        }
        if (readSECS2Data(&internalData._units) != true) {
            return false;
        }
        _internalData.push_back(internalData);
    }
    return true;
}
