//
// B1SECS2MessageS1F2Readable.cpp
//
// Library: B1SECS2
// Package: S1
// Module:  B1SECS2
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1SECS2.h"
#include "B1SECS2MessageS1F2Readable.h"

using namespace BnD;

B1SECS2MessageS1F2Readable::B1SECS2MessageS1F2Readable(bool wait, const uint8* data, uint32 dataLength, const B1SECS2DataManager* dataManager)
    : B1SECS2MessageReadable(wait, 1, 2, "recvMessageS1F2", data, dataLength, dataManager)
    , _mdln(dataManager)
    , _softRev(dataManager)
{
}

B1SECS2MessageS1F2Readable::~B1SECS2MessageS1F2Readable()
{
}

B1String B1SECS2MessageS1F2Readable::fullName()
{
    return "ON-LINE Data (D)";
}

void B1SECS2MessageS1F2Readable::implToString(B1String* string)
{
    appendStringDepth0List(_list.count(), string);
    if (isSentFromHost() != true) {
        appendStringDepth1(_mdln->toString(), string);
        appendStringDepth1(_softRev->toString(), string);
    }
    appendStringDepth0Closer(string);
}

bool B1SECS2MessageS1F2Readable::implReadData()
{
    /* ON-LINE Data (D) H->E
    <L 0>
    */

    /* ON-LINE Data (D) E->H
    <L 2>
        <MDLN>
        <SOFTREV>
    >
    */
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
