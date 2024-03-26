//
// B1SECS2MessageS2F15Readable.cpp
//
// Library: B1SECS2
// Package: S2
// Module:  B1SECS2
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1SECS2.h"
#include "B1SECS2MessageS2F15Readable.h"

using namespace BnD;

B1SECS2MessageS2F15Readable::B1SECS2MessageS2F15Readable(bool wait, const uint8* data, uint32 dataLength, const B1SECS2DataManager* dataManager)
    : B1SECS2MessageReadable(wait, 2, 15, "recvMessageS2F15", data, dataLength, dataManager)
    , _ecvs()
{
}

B1SECS2MessageS2F15Readable::~B1SECS2MessageS2F15Readable()
{
}

B1String B1SECS2MessageS2F15Readable::fullName()
{
    return "New Equipment Constant Request (ECS)";
}

void B1SECS2MessageS2F15Readable::implToString(B1String* string)
{
    appendStringDepth0List(_ecvs.size(), string);
    for (const auto& ecvsPair : _ecvs) {
        appendStringDepth1List(2, string);
        appendStringDepth2(ecvsPair.first->toString(), string);
        appendStringDepth2(ecvsPair.second->toString(), string);
        appendStringDepth1Closer(string);
    }
    appendStringDepth0Closer(string);
}

bool B1SECS2MessageS2F15Readable::implReadData()
{
    /* New Equipment Constant Request (ECS)
    W
    <L a
        <L 2
            <ECID>
            <ECV>
        >
    >
    */
    B1SECS2DataLIST list;
    if (readSECS2Data(&list) != true) {
        return false;
    }
    for (uint32 i = 0; i < list.count(); ++i) {
        if (readDummyList(2) != true) {
            return false;
        }
        B1SECS2DataECID ecID(dataManager());
        if (readSECS2Data(&ecID) != true) {
            return false;
        }
        auto ecv = readUnknownSECS2Data();
        if (ecv == NULL) {
            return false;
        }
        _ecvs.insert(std::make_pair(ecID, ecv));
    }
    return true;
}
