//
// B1SECS2MessageS2F13Readable.cpp
//
// Library: B1SECS2
// Package: S2
// Module:  B1SECS2
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1SECS2.h"
#include "B1SECS2MessageS2F13Readable.h"

using namespace BnD;

B1SECS2MessageS2F13Readable::B1SECS2MessageS2F13Readable(bool wait, const uint8* data, uint32 dataLength, const B1SECS2DataManager* dataManager)
    : B1SECS2MessageReadable(wait, 2, 13, "recvMessageS2F13", data, dataLength, dataManager)
    , _ecIDs()
{
}

B1SECS2MessageS2F13Readable::~B1SECS2MessageS2F13Readable()
{
}

B1String B1SECS2MessageS2F13Readable::fullName()
{
    return "Equipment Constant Request (ECR)";
}

void B1SECS2MessageS2F13Readable::implToString(B1String* string)
{
    appendStringDepth0List(_ecIDs.size(), string);
    for (size_t i = 0; i < _ecIDs.size(); ++i) {
        appendStringDepth1(_ecIDs[i]->toString(), string);
    }
    appendStringDepth0Closer(string);
}

bool B1SECS2MessageS2F13Readable::implReadData()
{
    /* Equipment Constant Request (ECR)
    W
    <L a
        <ECID>
    >
    */
    B1SECS2DataLIST list;
    if (readSECS2Data(&list) != true) {
        return false;
    }
    _ecIDs.reserve(list.count());
    for (uint32 i = 0; i < list.count(); ++i) {
        B1SECS2DataECID ecID(dataManager());
        if (readSECS2Data(&ecID) != true) {
            return false;
        }
        _ecIDs.push_back(ecID);
    }
    return true;
}
