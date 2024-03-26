//
// B1SECS2MessageS2F14Readable.cpp
//
// Library: B1SECS2
// Package: S2
// Module:  B1SECS2
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1SECS2.h"
#include "B1SECS2MessageS2F14Readable.h"

using namespace BnD;

B1SECS2MessageS2F14Readable::B1SECS2MessageS2F14Readable(bool wait, const uint8* data, uint32 dataLength, const B1SECS2DataManager* dataManager)
    : B1SECS2MessageReadable(wait, 2, 14, "recvMessageS2F14", data, dataLength, dataManager)
    , _ecvs()
{
}

B1SECS2MessageS2F14Readable::~B1SECS2MessageS2F14Readable()
{
}

B1String B1SECS2MessageS2F14Readable::fullName()
{
    return "Equipment Constant Data (ECD)";
}

void B1SECS2MessageS2F14Readable::implToString(B1String* string)
{
    appendStringDepth0List(_ecvs.size(), string);
    for (size_t i = 0; i < _ecvs.size(); ++i) {
        appendStringDepth1(_ecvs[i]->toString(), string);
    }
    appendStringDepth0Closer(string);
}

bool B1SECS2MessageS2F14Readable::implReadData()
{
    /* Equipment Constant Data (ECD)
    W
    <L a
        <ECV>
    >
    */
    B1SECS2DataLIST list;
    if (readSECS2Data(&list) != true) {
        return false;
    }
    _ecvs.reserve(list.count());
    for (uint32 i = 0; i < list.count(); ++i) {
        auto ecv = readUnknownSECS2Data();
        if (ecv == NULL) {
            return false;
        }
        _ecvs.push_back(ecv);
    }
    return true;
}
