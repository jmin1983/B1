//
// B1SECS2MessageS2F30Readable.cpp
//
// Library: B1SECS2
// Package: S2
// Module:  B1SECS2
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1SECS2.h"
#include "B1SECS2MessageS2F30Readable.h"

using namespace BnD;

B1SECS2MessageS2F30Readable::B1SECS2MessageS2F30Readable(bool wait, const uint8* data, uint32 dataLength, const B1SECS2DataManager* dataManager)
    : B1SECS2MessageReadable(wait, 2, 30, "recvMessageS2F30", data, dataLength, dataManager)
    , _internalData()
{
}

B1SECS2MessageS2F30Readable::~B1SECS2MessageS2F30Readable()
{
}

B1String B1SECS2MessageS2F30Readable::fullName()
{
    return "Equipment Constant Namelist (ECN)";
}

void B1SECS2MessageS2F30Readable::implToString(B1String* string)
{
    appendStringDepth0List(_internalData.size(), string);
    for (size_t i = 0; i < _internalData.size(); ++i) {
        appendStringDepth1List(6, string);
        appendStringDepth2(_internalData[i]._ecID->toString(), string);
        appendStringDepth2(_internalData[i]._ecName->toString(), string);
        appendStringDepth2(_internalData[i]._ecMin->get()->toString(), string);
        appendStringDepth2(_internalData[i]._ecMax->get()->toString(), string);
        appendStringDepth2(_internalData[i]._ecDef->get()->toString(), string);
        appendStringDepth2(_internalData[i]._units->toString(), string);
        appendStringDepth1Closer(string);
    }
    appendStringDepth0Closer(string);
}

bool B1SECS2MessageS2F30Readable::implReadData()
{
    /* Equipment Constant Namelist Request (ECNR)
    W
    <L n
        <L 6
            <ECID>
            <ECNAME>
            <ECMIN>
            <ECMAX>
            <ECDEF>
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
        if (readDummyList(6) != true) {
            return false;
        }
        InternalData internalData(dataManager());
        if (readSECS2Data(&internalData._ecID) != true) {
            return false;
        }
        if (readSECS2Data(&internalData._ecName) != true) {
            return false;
        }
        auto ecMin = readUnknownSECS2Data();
        if (ecMin == NULL) {
            return false;
        }
        internalData._ecMin.reset(new B1SECS2DataECMIN(ecMin));
        auto ecMax = readUnknownSECS2Data();
        if (ecMax == NULL) {
            return false;
        }
        internalData._ecMax.reset(new B1SECS2DataECMAX(ecMax));
        auto ecDef = readUnknownSECS2Data();
        if (ecDef == NULL) {
            return false;
        }
        internalData._ecDef.reset(new B1SECS2DataECDEF(ecDef));
        if (readSECS2Data(&internalData._units) != true) {
            return false;
        }
        _internalData.push_back(internalData);
    }
    return true;
}
