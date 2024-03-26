//
// B1SECS2MessageS1F16Writable.cpp
//
// Library: B1SECS2
// Package: S1
// Module:  B1SECS2
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1SECS2.h"
#include "B1SECS2MessageS1F16Writable.h"

using namespace BnD;

B1SECS2MessageS1F16Writable::B1SECS2MessageS1F16Writable(const B1SECS2DataOFLACK& oflAck)
    : B1SECS2MessageWritable(false, 1, 16, "sendMessageS1F16")
    , _oflAck(oflAck)
{
}

B1SECS2MessageS1F16Writable::~B1SECS2MessageS1F16Writable()
{
}

B1String B1SECS2MessageS1F16Writable::fullName()
{
    return "OFF-LINE Acknowledge (OFLA)";
}

bool B1SECS2MessageS1F16Writable::implWriteData(std::list<std::shared_ptr<B1SECS2Data> >* data)
{
    /* OFF-LINE Acknowledge (OFLA)
    <OFLACK>
    */
    addData(_oflAck, data);
    return true;
}
