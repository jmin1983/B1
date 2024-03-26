//
// B1SECS2MessageS1F21Readable.cpp
//
// Library: B1SECS2
// Package: S1
// Module:  B1SECS2
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1SECS2.h"
#include "B1SECS2MessageS1F21Readable.h"

using namespace BnD;

B1SECS2MessageS1F21Readable::B1SECS2MessageS1F21Readable(bool wait, const uint8* data, uint32 dataLength, const B1SECS2DataManager* dataManager)
    : B1SECS2MessageReadable(wait, 1, 21, "recvMessageS1F21", data, dataLength, dataManager)
    , _vIDs()
{
}

B1SECS2MessageS1F21Readable::~B1SECS2MessageS1F21Readable()
{
}

B1String B1SECS2MessageS1F21Readable::fullName()
{
    return "Data Variable Namelist Request (DVNR)";
}

void B1SECS2MessageS1F21Readable::implToString(B1String* string)
{
    appendStringDepth0List(_vIDs.size(), string);
    for (size_t i = 0; i < _vIDs.size(); ++i) {
        appendStringDepth1(_vIDs[i]->toString(), string);
    }
    appendStringDepth0Closer(string);
}

bool B1SECS2MessageS1F21Readable::implReadData()
{
    /* Data Variable Namelist Request (DVNR)
    W
    <L n
        <VID>
    >
    */
    B1SECS2DataLIST list;
    if (readSECS2Data(&list) != true) {
        return false;
    }
    _vIDs.reserve(list.count());
    for (uint32 i = 0; i < list.count(); ++i) {
        B1SECS2DataVID vID(dataManager());
        if (readSECS2Data(&vID) != true) {
            return false;
        }
        _vIDs.push_back(vID);
    }
    return true;
}
