//
// B1SECS2MessageS2F49Readable.cpp
//
// Library: B1SECS2
// Package: S2
// Module:  B1SECS2
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1SECS2.h"
#include "B1SECS2MessageS2F49Readable.h"

using namespace BnD;

B1SECS2MessageS2F49Readable::B1SECS2MessageS2F49Readable(bool wait, const uint8* data, uint32 dataLength, const B1SECS2DataManager* dataManager)
    : B1SECS2MessageReadable(wait, 2, 49, "recvMessageS2F49", data, dataLength, dataManager)
    , _dataID(dataManager)
    , _objSpec(dataManager)
    , _rcmd(dataManager)
    , _cps()
{
}

B1SECS2MessageS2F49Readable::~B1SECS2MessageS2F49Readable()
{
}

B1String B1SECS2MessageS2F49Readable::fullName()
{
    return "Enhanced Remote Command Send (S2F49)";
}

void B1SECS2MessageS2F49Readable::implToString(B1String* string)
{
    appendStringDepth0List(4, string);
    appendStringDepth1(_dataID->toString(), string);
    appendStringDepth1(_objSpec->toString(), string);
    appendStringDepth1(_rcmd->toString(), string);
    appendStringDepth1List(_cps.size(), string);
    for (const auto& cpsPair : _cps) {
        appendStringDepth2List(2, string);
        appendStringDepth3(cpsPair.first->toString(), string);
        appendStringDepth3(cpsPair.second->toString(), string);
        appendStringDepth2Closer(string);
    }
    appendStringDepth1Closer(string);
    appendStringDepth0Closer(string);
}

bool B1SECS2MessageS2F49Readable::implReadData()
{
    /* Enhanced Remote Command Send
    W
    <L 4
        <DATAID>
        <OBJSPEC>
        <RCMD>
        <L n
            <L 2
                <CPNAME>
                <CEPVAL>
            >
        >
    >
    */
    if (readDummyList(4) != true) {
        return false;
    }
    if (readSECS2Data(&_dataID) != true) {
        return false;
    }
    if (readSECS2Data(&_objSpec) != true) {
        return false;
    }
    if (readSECS2Data(&_rcmd) != true) {
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
        B1SECS2DataCPNAME cpName(dataManager());
        if (readSECS2Data(&cpName) != true) {
            return false;
        }
        B1SECS2DataCEPVAL cepVal(readUnknownSECS2Data());
        if (cepVal.data() == NULL) {
            return false;
        }
        if (cepVal->isFormatList()) {
            if (readSECS2DataList(std::dynamic_pointer_cast<B1SECS2DataLIST>(cepVal.data()), &cepVal._list) != true) {
                return false;
            }
        }
        _cps.insert(std::make_pair(cpName, cepVal));
    }
    return true;
}
