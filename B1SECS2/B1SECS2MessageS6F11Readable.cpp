//
// B1SECS2MessageS6F11Readable.cpp
//
// Library: B1SECS2
// Package: S6
// Module:  B1SECS2
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1SECS2.h"
#include "B1SECS2MessageS6F11Readable.h"

using namespace BnD;

B1SECS2MessageS6F11Readable::B1SECS2MessageS6F11Readable(bool wait, const uint8* data, uint32 dataLength, const B1SECS2DataManager* dataManager)
    : B1SECS2MessageReadable(wait, 6, 11, "recvMessageS6F11", data, dataLength, dataManager)
    , _dataID(dataManager)
    , _ceID(dataManager)
    , _reportsData()
{
}

B1SECS2MessageS6F11Readable::~B1SECS2MessageS6F11Readable()
{
}

B1String B1SECS2MessageS6F11Readable::fullName()
{
    return "Event Report Send (ERS)";
}

void B1SECS2MessageS6F11Readable::implToString(B1String* string)
{
    appendStringDepth0List(3, string);
    appendStringDepth1(_dataID->toString(), string);
    appendStringDepth1(_ceID->toString(), string);
    appendStringDepth1List(_reportsData.size(), string);
    for (const auto& reportsDataPair : _reportsData) {
        appendStringDepth2List(2, string);
        appendStringDepth3(reportsDataPair.first->toString(), string);
        const std::vector<B1SECS2DataV>& vs = reportsDataPair.second;
        appendStringDepth3List(vs.size(), string);
        for (const auto& v : vs) {
            appendStringDepth4(v->toString(), string);
        }
        appendStringDepth3Closer(string);
        appendStringDepth2Closer(string);
    }
    appendStringDepth1Closer(string);
    appendStringDepth0Closer(string);
}

bool B1SECS2MessageS6F11Readable::implReadData()
{
    /* Event Report Send (ERS)
    <L 3 
        <DATAID>
        <CEID>
        <L a
            <L 2
                <RPTID>
                <L b
                    <V>
                >
            >
        >
    >
    */
    if (readDummyList(3) != true) {
        return false;
    }
    if (readSECS2Data(&_dataID) != true) {
        return false;
    }
    if (readSECS2Data(&_ceID) != true) {
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
        B1SECS2DataRPTID rptID(dataManager());
        if (readSECS2Data(&rptID) != true) {
            return false;
        }
        B1SECS2DataLIST vList;
        if (readSECS2Data(&vList) != true) {
            return false;
        }
        std::vector<B1SECS2DataV> vs;
        vs.reserve(vList.count());
        for (uint32 j = 0; j < vList.count(); ++j) {
            auto data = readUnknownSECS2Data();
            if (data == NULL) {
                return false;
            }
            if (data->isFormatList()) {
                B1SECS2DataV v(data);
                readSECS2DataList(std::dynamic_pointer_cast<B1SECS2DataLIST>(data), &v._list);
                vs.push_back(v);
            }
            else {
                vs.push_back(data);
            }
        }
        _reportsData.insert(std::make_pair(rptID, vs));
    }
    return true;
}
