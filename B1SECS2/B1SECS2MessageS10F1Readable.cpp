//
// B1SECS2MessageS10F1Readable.cpp
//
// Library: B1SECS2
// Package: S10
// Module:  B1SECS2
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1SECS2.h"
#include "B1SECS2MessageS10F1Readable.h"

using namespace BnD;

B1SECS2MessageS10F1Readable::B1SECS2MessageS10F1Readable(bool wait, const uint8* data, uint32 dataLength, const B1SECS2DataManager* dataManager)
    : B1SECS2MessageReadable(wait, 10, 1, "recvMessageS10F1", data, dataLength, dataManager)
    , _tid(dataManager)
    , _text(dataManager)
{
}

B1SECS2MessageS10F1Readable::~B1SECS2MessageS10F1Readable()
{
}

B1String B1SECS2MessageS10F1Readable::fullName()
{
    return "Terminal Request (TRN)";
}

void B1SECS2MessageS10F1Readable::implToString(B1String* string)
{
    appendStringDepth0List(2, string);
    appendStringDepth1(_tid->toString(), string);
    appendStringDepth1(_text->toString(), string);
    appendStringDepth0Closer(string);
}

bool B1SECS2MessageS10F1Readable::implReadData()
{
    /* Terminal Request (TRN)
    <L 2
        <TID>
        <TEXT>
    >
    */
    if (readDummyList(2) != true) {
        return false;
    }
    if (readSECS2Data(&_tid) != true) {
        return false;
    }
    if (readSECS2Data(&_text) != true) {
        return false;
    }
    return true;
}
