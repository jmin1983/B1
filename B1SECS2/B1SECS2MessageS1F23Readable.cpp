//
// B1SECS2MessageS1F23Readable.cpp
//
// Library: B1SECS2
// Package: S1
// Module:  B1SECS2
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1SECS2.h"
#include "B1SECS2MessageS1F23Readable.h"

using namespace BnD;

B1SECS2MessageS1F23Readable::B1SECS2MessageS1F23Readable(bool wait, const uint8* data, uint32 dataLength, const B1SECS2DataManager* dataManager)
    : B1SECS2MessageReadable(wait, 1, 23, "recvMessageS1F23", data, dataLength, dataManager)
    , _ceIDs()
{
}

B1SECS2MessageS1F23Readable::~B1SECS2MessageS1F23Readable()
{
}

B1String B1SECS2MessageS1F23Readable::fullName()
{
    return "Collection Event Namelist Request (CENR)";
}

void B1SECS2MessageS1F23Readable::implToString(B1String* string)
{
    appendStringDepth0List(_ceIDs.size(), string);
    for (size_t i = 0; i < _ceIDs.size(); ++i) {
        appendStringDepth1(_ceIDs[i]->toString(), string);
    }
    appendStringDepth0Closer(string);
}

bool B1SECS2MessageS1F23Readable::implReadData()
{
    /* Collection Event Namelist Request (CENR)
    W
    <L n
        <CEID>
    >
    */
    B1SECS2DataLIST list;
    if (readSECS2Data(&list) != true) {
        return false;
    }
    _ceIDs.reserve(list.count());
    for (uint32 i = 0; i < list.count(); ++i) {
        B1SECS2DataCEID ceID(dataManager());
        if (readSECS2Data(&ceID) != true) {
            return false;
        }
        _ceIDs.push_back(ceID);
    }
    return true;
}
