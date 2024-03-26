//
// B1SECS2MessageS2F33Readable.cpp
//
// Library: B1SECS2
// Package: S2
// Module:  B1SECS2
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1SECS2.h"
#include "B1SECS2MessageS2F33Readable.h"

using namespace BnD;

B1SECS2MessageS2F33Readable::B1SECS2MessageS2F33Readable(bool wait, const uint8* data, uint32 dataLength, const B1SECS2DataManager* dataManager)
    : B1SECS2MessageReadable(wait, 2, 33, "recvMessageS2F33", data, dataLength, dataManager)
    , _dataID(dataManager)
    , _reportData()
{
}

B1SECS2MessageS2F33Readable::~B1SECS2MessageS2F33Readable()
{
}

B1String B1SECS2MessageS2F33Readable::fullName()
{
    return "Define Report (DR)";
}

void B1SECS2MessageS2F33Readable::implToString(B1String* string)
{
    appendStringDepth0List(2, string);
    appendStringDepth1(_dataID->toString(), string);
    appendStringDepth1List(_reportData.size(), string);
    for (const auto& reportDataPair : _reportData) {
        appendStringDepth2List(2, string);
        appendStringDepth3(reportDataPair.first->toString(), string);
        const std::vector<B1SECS2DataVID>& vIDs = reportDataPair.second;
        appendStringDepth3List(vIDs.size(), string);
        for (const auto& vID : vIDs) {
            appendStringDepth4(vID->toString(), string);
        }
        appendStringDepth3Closer(string);
        appendStringDepth2Closer(string);
    }
    appendStringDepth1Closer(string);
    appendStringDepth0Closer(string);
}

bool B1SECS2MessageS2F33Readable::implReadData()
{
    /* Define Report (DR)
    W
    <L 2
        <DATAID>
        <L a                //  a==0 means delete all reports and event links.
            <L 2
                <RPTID>
                <L b        //  b==0 means delete the RPTID type and its event links
                    <VID>
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
    if (list.count() == 0) {
        _reportData.clear();
    }
    else {
        for (uint32 i = 0; i < list.count(); ++i) {
            if (readDummyList(2) != true) {
                return false;
            }
            B1SECS2DataRPTID rptID(dataManager());
            if (readSECS2Data(&rptID) != true) {
                return false;
            }
            B1SECS2DataLIST reportList;
            if (readSECS2Data(&reportList) != true) {
                return false;
            }

            std::vector<B1SECS2DataVID> vIDs;
            vIDs.reserve(reportList.count());
            for (uint32 j = 0; j < reportList.count(); ++j) {
                B1SECS2DataVID vID(dataManager());
                if (readSECS2Data(&vID) != true) {
                    return false;
                }
                vIDs.push_back(vID);
            }
            _reportData.insert(std::make_pair(rptID, vIDs));
        }
    }
    return true;
}
