//
// B1SECS2MessageS10F3Readable.cpp
//
// Library: B1SECS2
// Package: S10
// Module:  B1SECS2
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1SECS2.h"
#include "B1SECS2MessageS10F3Readable.h"

using namespace BnD;

B1SECS2MessageS10F3Readable::B1SECS2MessageS10F3Readable(bool wait, const uint8* data, uint32 dataLength, const B1SECS2DataManager* dataManager)
    : B1SECS2MessageReadable(wait, 10, 3, "recvMessageS10F3", data, dataLength, dataManager)
    , _tid(dataManager)
    , _text(dataManager)
{
}

B1SECS2MessageS10F3Readable::~B1SECS2MessageS10F3Readable()
{
}

B1String B1SECS2MessageS10F3Readable::fullName()
{
    return "Terminal Display, Single (VTN)";
}

void B1SECS2MessageS10F3Readable::implToString(B1String* string)
{
    appendStringDepth0List(2, string);
    appendStringDepth1(_tid->toString(), string);
    appendStringDepth1(_text->toString(), string);
    appendStringDepth0Closer(string);
}

bool B1SECS2MessageS10F3Readable::implReadData()
{
    /* Terminal Display, Single (VTN)
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
    {
        auto textData = static_cast<B1SECS2DataASCII*>(_text.get());
        B1String temp = textData->data().copy();
        temp.replace("%", "%%");
        textData->setData(std::move(temp));
    }
    return true;
}
