//
// B1SECS2MessageS1F11Readable.cpp
//
// Library: B1SECS2
// Package: S1
// Module:  B1SECS2
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1SECS2.h"
#include "B1SECS2MessageS1F11Readable.h"

using namespace BnD;

B1SECS2MessageS1F11Readable::B1SECS2MessageS1F11Readable(bool wait, const uint8* data, uint32 dataLength, const B1SECS2DataManager* dataManager)
    : B1SECS2MessageReadable(wait, 1, 11, "recvMessageS1F11", data, dataLength, dataManager)
    , _svIDs()
{
}

B1SECS2MessageS1F11Readable::~B1SECS2MessageS1F11Readable()
{
}

B1String B1SECS2MessageS1F11Readable::fullName()
{
    return "Status Variable Namelist Request (SVNR)";
}

void B1SECS2MessageS1F11Readable::implToString(B1String* string)
{
    appendStringDepth0List(_svIDs.size(), string);
    for (size_t i = 0; i < _svIDs.size(); ++i) {
        appendStringDepth1(_svIDs[i]->toString(), string);
    }
    appendStringDepth0Closer(string);
}

bool B1SECS2MessageS1F11Readable::implReadData()
{
    /* Status Variable Namelist Request (SVNR)
    W
    <L n
        <SVID>
    >
    */
    B1SECS2DataLIST list;
    if (readSECS2Data(&list) != true) {
        return false;
    }
    _svIDs.reserve(list.count());
    for (uint32 i = 0; i < list.count(); ++i) {
        B1SECS2DataSVID svID(dataManager());
        if (readSECS2Data(&svID) != true) {
            return false;
        }
        _svIDs.push_back(svID);
    }
    return true;
}
