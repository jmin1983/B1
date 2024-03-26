//
// B1SECS2MessageS1F14Readable.cpp
//
// Library: B1SECS2
// Package: S1
// Module:  B1SECS2
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1SECS2.h"
#include "B1SECS2MessageS1F14Readable.h"

using namespace BnD;

B1SECS2MessageS1F14Readable::B1SECS2MessageS1F14Readable(bool wait, const uint8* data, uint32 dataLength, const B1SECS2DataManager* dataManager)
    : B1SECS2MessageReadable(wait, 1, 14, "recvMessageS1F14", data, dataLength, dataManager)
    , _commAck(dataManager)
    , _list()
    , _mdln(dataManager)
    , _softRev(dataManager)
{
}

B1SECS2MessageS1F14Readable::~B1SECS2MessageS1F14Readable()
{
}

B1String B1SECS2MessageS1F14Readable::fullName()
{
    return "Establish Communications Request Acknowledge (CRA)";
}

void B1SECS2MessageS1F14Readable::implToString(B1String* string)
{
    appendStringDepth0List(2, string);
    appendStringDepth1(_commAck->toString(), string);
    appendStringDepth1List(_list.count(), string);
    if (isSentFromHost() != true) {
        appendStringDepth2(_mdln->toString(), string);
        appendStringDepth2(_softRev->toString(), string);
    }
    appendStringDepth1Closer(string);
    appendStringDepth0Closer(string);
}

bool B1SECS2MessageS1F14Readable::implReadData()
{
    /* Establish Communications Request Acknowledge (CRA)
    <L 2
        <COMMACK>
        <L 2                Exception: The host sends a zero-length list to the equipment.
            <MDLN>
            <SOFTREV>
        >
    >
    */
    if (readDummyList(2) != true) {
        return false;
    }
    if (readSECS2Data(&_commAck) != true) {
        return false;
    }
    if (readSECS2Data(&_list) != true) {
        return false;
    }
    if (isSentFromHost() != true) {
        if (readSECS2Data(&_mdln) != true) {
            return false;
        }
        if (readSECS2Data(&_softRev) != true) {
            return false;
        }
    }
    return true;
}
