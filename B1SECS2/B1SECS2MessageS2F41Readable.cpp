//
// B1SECS2MessageS2F41Readable.cpp
//
// Library: B1SECS2
// Package: S2
// Module:  B1SECS2
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1SECS2.h"
#include "B1SECS2MessageS2F41Readable.h"

using namespace BnD;

B1SECS2MessageS2F41Readable::B1SECS2MessageS2F41Readable(bool wait, const uint8* data, uint32 dataLength, const B1SECS2DataManager* dataManager)
    : B1SECS2MessageReadable(wait, 2, 41, "recvMessageS2F41", data, dataLength, dataManager)
    , _rcmd(dataManager)
    , _cps()
{
}

B1SECS2MessageS2F41Readable::~B1SECS2MessageS2F41Readable()
{
}

B1String B1SECS2MessageS2F41Readable::fullName()
{
    return "Host Command Send (HCS)";
}

void B1SECS2MessageS2F41Readable::implToString(B1String* string)
{
    appendStringDepth0List(2, string);
    appendStringDepth1(_rcmd->toString(), string);
    appendStringDepth1List(_cps.size(), string);
    for (const auto& cpsPair : _cps) {
        appendStringDepth2List(2, string);
        appendStringDepth3(cpsPair.first->toString(), string);
        appendStringDepth3(cpsPair.second->toString(), string);
        appendStringDepth2Closer(string);
    }
    appendStringDepth1Closer(string);
    appendStringDepth0Closer(string);
}

bool B1SECS2MessageS2F41Readable::implReadData()
{
    /* Host Command Send (HCS)
    W
    <L 2
        <RCMD>
        <L n
            <L 2
                <CPNAME>
                <CPVAL>
            >
        >
    >
    */
    if (readDummyList(2) != true) {
        return false;
    }
    if (readSECS2Data(&_rcmd) != true) {
        return false;
    }
    B1SECS2DataLIST list;
    if (readSECS2Data(&list) != true) {
        return false;
    }
    for (uint32 i = 0; i < list.count(); ++i) {
        if (readDummyList(2) != true) {
            return false;
        }
        B1SECS2DataCPNAME cpName(dataManager());
        if (readSECS2Data(&cpName) != true) {
            return false;
        }
        auto cpVal = readUnknownSECS2Data();
        if (cpVal == NULL) {
            return false;
        }
        _cps.insert(std::make_pair(cpName, cpVal));
    }
    return true;
}
