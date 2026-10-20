//
// B1SECS2MessageS10F5Readable.cpp
//
// Library: B1SECS2
// Package: S10
// Module:  B1SECS2
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1SECS2.h"
#include "B1SECS2MessageS10F5Readable.h"

using namespace BnD;

B1SECS2MessageS10F5Readable::B1SECS2MessageS10F5Readable(bool wait, const uint8* data, uint32 dataLength, const B1SECS2DataManager* dataManager)
    : B1SECS2MessageReadable(wait, 10, 5, "recvMessageS10F5", data, dataLength, dataManager)
    , _tid(dataManager)
    , _texts()
{
}

B1SECS2MessageS10F5Readable::~B1SECS2MessageS10F5Readable()
{
}

B1String B1SECS2MessageS10F5Readable::fullName()
{
    return "Terminal Display, Multi-Block (VTN)";
}

void B1SECS2MessageS10F5Readable::implToString(B1String* string)
{
    appendStringDepth0List(2, string);
    appendStringDepth1(_tid->toString(), string);
    appendStringDepth1List(_texts.size(), string);
    for (const auto& text : _texts) {
        appendStringDepth2(text->toString(), string);
    }
    appendStringDepth1Closer(string);
    appendStringDepth0Closer(string);
}

bool B1SECS2MessageS10F5Readable::implReadData()
{
    /* Terminal Display, Multi-Block (VTN)
    <L 2
        <TID>
        <L n
            <TEXT>
        >
    >
    */
    if (readDummyList(2) != true) {
        return false;
    }
    if (readSECS2Data(&_tid) != true) {
        return false;
    }
    B1SECS2DataLIST vList;
    if (readSECS2Data(&vList) != true) {
        return false;
    }
    _texts.clear();
    _texts.reserve(vList.count());
    for (uint32 i = 0; i < vList.count(); ++i) {
        B1SECS2DataTEXT text(dataManager());
        if (readSECS2Data(&text) != true) {
            return false;
        }
        if (text.data()->isFormatASCII()) {
            auto textData = static_cast<B1SECS2DataASCII*>(text.get());
            B1String temp = textData->data().copy();
            temp.replace("%", "%%");
            textData->setData(std::move(temp));
            _texts.push_back(std::move(text));
        }
    }
    return true;
}
