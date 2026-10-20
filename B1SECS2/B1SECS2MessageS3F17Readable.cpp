//
// B1SECS2MessageS3F17Readable.cpp
//
// Library: B1SECS2
// Package: S3
// Module:  B1SECS2
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1SECS2.h"
#include "B1SECS2MessageS3F17Readable.h"

using namespace BnD;

B1SECS2MessageS3F17Readable::B1SECS2MessageS3F17Readable(bool wait, const uint8* data, uint32 dataLength, const B1SECS2DataManager* dataManager)
    : B1SECS2MessageReadable(wait, 3, 17, "recvMessageS3F17", data, dataLength, dataManager)
    , _dataID(dataManager)
    , _carrierAction(dataManager)
    , _carrierID(dataManager)
    , _ptn(dataManager)
    , _cattrData()
{
}

B1SECS2MessageS3F17Readable::~B1SECS2MessageS3F17Readable()
{
}

B1String B1SECS2MessageS3F17Readable::fullName()
{
    return "Carrier Action Request (S3F17)";
}

void B1SECS2MessageS3F17Readable::implToString(B1String* string)
{
    appendStringDepth0List(5, string);
    appendStringDepth1(_dataID->toString(), string);
    appendStringDepth1(_carrierAction->toString(), string);
    appendStringDepth1(_carrierID->toString(), string);
    appendStringDepth1(_ptn->toString(), string);
    appendStringDepth1List(_cattrData.size(), string);
    for (const auto& cattrDataPair : _cattrData) {
        appendStringDepth2List(2, string);
        appendStringDepth3(cattrDataPair.first->toString(), string);
        appendStringDepth3(cattrDataPair.second->toString(), string);
        appendStringDepth2Closer(string);
    }
    appendStringDepth1Closer(string);
    appendStringDepth0Closer(string);
}

bool B1SECS2MessageS3F17Readable::implReadData()
{
    /* Carrier Action Request (S3F17)
    <L 5
        <DATAID>
        <CARRIERACTION>
        <CARRIERID>
        <PTN>
        <L n
            <L 2
                <CATTRID>
                <CATTRDATA>
            >
        >
    >
    */
    if (readDummyList(5) != true) {
        return false;
    }
    if (readSECS2Data(&_dataID) != true) {
        return false;
    }
    if (readSECS2Data(&_carrierAction) != true) {
        return false;
    }
    if (readSECS2Data(&_carrierID) != true) {
        return false;
    }
    if (readSECS2Data(&_ptn) != true) {
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
        B1SECS2DataCATTRID cattrID(dataManager());
        if (readSECS2Data(&cattrID) != true) {
            return false;
        }
        B1SECS2DataCATTRDATA cattrData(dataManager());
        if (readSECS2Data(&cattrData) != true) {
            return false;
        }
        _cattrData.emplace(cattrID, cattrData);
    }
    return true;
}
