//
// B1SECS2MessageS3F18Writable.cpp
//
// Library: B1SECS2
// Package: S3
// Module:  B1SECS2
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1SECS2.h"
#include "B1SECS2MessageS3F18Writable.h"

using namespace BnD;

B1SECS2MessageS3F18Writable::B1SECS2MessageS3F18Writable(const B1SECS2DataCAACK& acAck, const std::map<B1SECS2DataERRCODE, B1SECS2DataERRTEXT>& errorText)
    : B1SECS2MessageWritable(false, 3, 18, "sendMessageS3F18")
    , _acAck(acAck)
    , _errorText(errorText)
{
}

B1SECS2MessageS3F18Writable::~B1SECS2MessageS3F18Writable()
{
}

B1String B1SECS2MessageS3F18Writable::fullName()
{
    return "Carrier Action Ack (S3F18)";
}

bool B1SECS2MessageS3F18Writable::implWriteData(std::list<std::shared_ptr<B1SECS2Data> >* data)
{
    /* Carrier Action Ack (S3F18)
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
    addList(2, data);
    addData(_acAck, data);
    addList(_errorText.size(), data);
    for (const auto& errorTextPair : _errorText) {
        addList(2, data);
        addData(errorTextPair.first, data);
        addData(errorTextPair.second, data);
    }
    return true;
}
