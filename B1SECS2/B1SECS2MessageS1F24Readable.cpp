//
// B1SECS2MessageS1F24Readable.cpp
//
// Library: B1SECS2
// Package: S1
// Module:  B1SECS2
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1SECS2.h"
#include "B1SECS2MessageS1F24Readable.h"

using namespace BnD;

B1SECS2MessageS1F24Readable::B1SECS2MessageS1F24Readable(bool wait, const uint8* data, uint32 dataLength, const B1SECS2DataManager* dataManager)
    : B1SECS2MessageReadable(wait, 1, 24, "recvMessageS1F24", data, dataLength, dataManager)
    , _internalData()
{
}

B1SECS2MessageS1F24Readable::~B1SECS2MessageS1F24Readable()
{
}

B1String B1SECS2MessageS1F24Readable::fullName()
{
    return "Collection Event Namelist (CEN)";
}

void B1SECS2MessageS1F24Readable::implToString(B1String* string)
{
    appendStringDepth0List(_internalData.size(), string);
    for (size_t i = 0; i < _internalData.size(); ++i) {
        appendStringDepth1List(3, string);
        appendStringDepth2(_internalData[i]._ceID->toString(), string);
        appendStringDepth2(_internalData[i]._ceName->toString(), string);
        appendStringDepth2List(_internalData[i]._vIDs.size(), string);
        for (size_t j = 0; j < _internalData[i]._vIDs.size(); ++j) {
            appendStringDepth3(_internalData[i]._vIDs[j]->toString(), string);
        }
        appendStringDepth2Closer(string);
        appendStringDepth1Closer(string);
    }
    appendStringDepth0Closer(string);
}

bool B1SECS2MessageS1F24Readable::implReadData()
{
    /* Collection Event Namelist (CEN)
    W
    <L n
        <L 3
            <CEID>
            <CENAME>
            <L a
                <VID>
            >
        >
    >
    */
    B1SECS2DataLIST list;
    if (readSECS2Data(&list) != true) {
        return false;
    }
    _internalData.reserve(list.count());
    for (uint32 i = 0; i < list.count(); ++i) {
        if (readDummyList(3) != true) {
            return false;
        }
        InternalData internalData(dataManager());
        if (readSECS2Data(&internalData._ceID) != true) {
            return false;
        }
        if (readSECS2Data(&internalData._ceName) != true) {
            return false;
        }
        B1SECS2DataLIST vIDList;
        if (readSECS2Data(&vIDList) != true) {
            return false;
        }
        internalData._vIDs.reserve(vIDList.count());
        for (uint32 j = 0; i < vIDList.count(); ++j) {
            B1SECS2DataVID vID(dataManager());
            if (readSECS2Data(&vID) != true) {
                return false;
            }
            internalData._vIDs.push_back(vID);
        }
        _internalData.push_back(internalData);
    }
    return true;
}
