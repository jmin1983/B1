//
// B1SECS2MessageS3F18Readable.cpp
//
// Library: B1SECS2
// Package: S3
// Module:  B1SECS2
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1SECS2.h"
#include "B1SECS2MessageS3F18Readable.h"

using namespace BnD;

B1SECS2MessageS3F18Readable::B1SECS2MessageS3F18Readable(bool wait, const uint8* data, uint32 dataLength, const B1SECS2DataManager* dataManager)
    : B1SECS2MessageReadable(wait, 3, 18, "recvMessageS3F18", data, dataLength, dataManager)
    , _acAck(dataManager)
    , _errorText()
{
}

B1SECS2MessageS3F18Readable::~B1SECS2MessageS3F18Readable()
{
}

B1String B1SECS2MessageS3F18Readable::fullName()
{
    return "Carrier Action Ack (S3F18)";
}

void B1SECS2MessageS3F18Readable::implToString(B1String* string)
{
    appendStringDepth0List(2, string);
    appendStringDepth1(_acAck->toString(), string);
    appendStringDepth1List(_errorText.size(), string);
    for (const auto& errorTextPair : _errorText) {
        appendStringDepth2List(2, string);
        appendStringDepth3(errorTextPair.first->toString(), string);
        appendStringDepth3(errorTextPair.second->toString(), string);
        appendStringDepth2Closer(string);
    }
    appendStringDepth1Closer(string);
    appendStringDepth0Closer(string);
}

bool B1SECS2MessageS3F18Readable::implReadData()
{
    /* Carrier Action Ack (S3F18)
    W
    <L 2
        <CAACK>
        <L n
            <L 2
                <ERRCODE>
                <ERRTEXT>
            >
        >
    >
    */
    if (readDummyList(2) != true) {
        return false;
    }
    if (readSECS2Data(&_acAck) != true) {
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
        B1SECS2DataERRCODE errCode(dataManager());
        if (readSECS2Data(&errCode) != true) {
            return false;
        }
        B1SECS2DataERRTEXT errText(dataManager());
        if (readSECS2Data(&errText) != true) {
            return false;
        }
        _errorText.emplace(errCode, errText);
    }
    return true;
}
