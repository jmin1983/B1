//
// B1SECS2MessageS2F35Readable.cpp
//
// Library: B1SECS2
// Package: S2
// Module:  B1SECS2
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1SECS2.h"
#include "B1SECS2MessageS2F35Readable.h"

using namespace BnD;

B1SECS2MessageS2F35Readable::B1SECS2MessageS2F35Readable(bool wait, const uint8* data, uint32 dataLength, const B1SECS2DataManager* dataManager)
    : B1SECS2MessageReadable(wait, 2, 35, "recvMessageS2F35", data, dataLength, dataManager)
    , _dataID(dataManager)
    , _linkEventData()
{
}

B1SECS2MessageS2F35Readable::~B1SECS2MessageS2F35Readable()
{
}

B1String B1SECS2MessageS2F35Readable::fullName()
{
    return "Link Event Report (LER)";
}

void B1SECS2MessageS2F35Readable::implToString(B1String* string)
{
    appendStringDepth0List(2, string);
    appendStringDepth1(_dataID->toString(), string);
    appendStringDepth1List(_linkEventData.size(), string);
    for (const auto& linkEventDataPair : _linkEventData) {
        appendStringDepth2List(2, string);
        appendStringDepth3(linkEventDataPair.first->toString(), string);
        const std::vector<B1SECS2DataRPTID>& rptIDs = linkEventDataPair.second;
        appendStringDepth3List(rptIDs.size(), string);
        for (const auto& rptID : rptIDs) {
            appendStringDepth4(rptID->toString(), string);
        }
        appendStringDepth3Closer(string);
        appendStringDepth2Closer(string);
    }
    appendStringDepth1Closer(string);
    appendStringDepth0Closer(string);
}

bool B1SECS2MessageS2F35Readable::implReadData()
{
    /* Link Event Report (LER)
    W
    <L 2
        <DATAID>
        <L a
            <L 2
                <CEID>
                <L b        //  b==0 disassociates the CEID from all report links. The message links reports to an event, it does not imply a particular output sequence in the event report.
                    <RPRTID>
                >
            >
        >
    >
    */
    if (readDummyList(2) != true) {
        return false;
    }
    if (readSECS2Data(&_dataID) != true) {
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
        B1SECS2DataCEID ceID(dataManager());
        if (readSECS2Data(&ceID) != true) {
            return false;
        }
        B1SECS2DataLIST ceList;
        if (readSECS2Data(&ceList) != true) {
            return false;
        }
        std::vector<B1SECS2DataRPTID> rptIDs;
        rptIDs.reserve(ceList.count());
        for (uint32 j = 0; j < ceList.count(); ++j) {
            B1SECS2DataRPTID rptID(dataManager());
            if (readSECS2Data(&rptID) != true) {
                return false;
            }
            rptIDs.push_back(rptID);
        }
        _linkEventData.insert(std::make_pair(ceID, rptIDs));
    }
    return true;
}
