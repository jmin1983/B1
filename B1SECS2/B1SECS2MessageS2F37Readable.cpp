//
// B1SECS2MessageS2F37Readable.cpp
//
// Library: B1SECS2
// Package: S2
// Module:  B1SECS2
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1SECS2.h"
#include "B1SECS2MessageS2F37Readable.h"

using namespace BnD;

B1SECS2MessageS2F37Readable::B1SECS2MessageS2F37Readable(bool wait, const uint8* data, uint32 dataLength, const B1SECS2DataManager* dataManager)
    : B1SECS2MessageReadable(wait, 2, 37, "recvMessageS2F37", data, dataLength, dataManager)
    , _ceed(dataManager)
    , _ceIDs()
{
}

B1SECS2MessageS2F37Readable::~B1SECS2MessageS2F37Readable()
{
}

B1String B1SECS2MessageS2F37Readable::fullName()
{
    return "Enable/Disable Event Report (EDER)";
}

void B1SECS2MessageS2F37Readable::implToString(B1String* string)
{
    appendStringDepth0List(2, string);
    appendStringDepth1(_ceed->toString(), string);
    appendStringDepth1List(_ceIDs.size(), string);
    for (const auto& ceID : _ceIDs) {
        appendStringDepth2(ceID->toString(), string);
    }
    appendStringDepth1Closer(string);
    appendStringDepth0Closer(string);
}

bool B1SECS2MessageS2F37Readable::implReadData()
{
    /* Enable/Disable Event Report (EDER)
    W
    <L 2
        <CEED>
        <L n        //  n==0 means all CEIDs.
            <CEID>
        >
    >
    */
    if (readDummyList(2) != true) {
        return false;
    }
    if (readSECS2Data(&_ceed) != true) {
        return false;
    }
    B1SECS2DataLIST list;
    if (readSECS2Data(&list) != true) {
        return false;
    }
    _ceIDs.reserve(list.count());
    for (uint32 i = 0; i < list.count(); ++i) {
        B1SECS2DataCEID ceID(dataManager());
        if (readSECS2Data(&ceID) != true) {
            return false;
        }
        _ceIDs.push_back(ceID);
    }
    return true;
}
