//
// B1SECS2MessageS2F32Writable.cpp
//
// Library: B1SECS2
// Package: S2
// Module:  B1SECS2
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1SECS2.h"
#include "B1SECS2MessageS2F32Writable.h"

using namespace BnD;

B1SECS2MessageS2F32Writable::B1SECS2MessageS2F32Writable(const B1SECS2DataTIACK& tiAck)
    : B1SECS2MessageWritable(false, 2, 32, "sendMessageS2F32")
    , _tiAck(tiAck)
{
}

B1SECS2MessageS2F32Writable::~B1SECS2MessageS2F32Writable()
{
}

B1String B1SECS2MessageS2F32Writable::fullName()
{
    return "Date and Time Set Acknowledge (DTA)";
}

bool B1SECS2MessageS2F32Writable::implWriteData(std::list<std::shared_ptr<B1SECS2Data> >* data)
{
    /* Date and Time Set Acknowledge (DTA)
    <TIACK>
    */
    addData(_tiAck, data);
    return true;
}
