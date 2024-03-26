//
// B1SECS2MessageS2F34Writable.cpp
//
// Library: B1SECS2
// Package: S2
// Module:  B1SECS2
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1SECS2.h"
#include "B1SECS2MessageS2F34Writable.h"

using namespace BnD;

B1SECS2MessageS2F34Writable::B1SECS2MessageS2F34Writable(const B1SECS2DataDRACK& drAck)
    : B1SECS2MessageWritable(false, 2, 34, "sendMessageS2F34")
    , _drAck(drAck)
{
}

B1SECS2MessageS2F34Writable::~B1SECS2MessageS2F34Writable()
{
}

B1String B1SECS2MessageS2F34Writable::fullName()
{
    return "Define Report Acknowledge (DRA)";
}

bool B1SECS2MessageS2F34Writable::implWriteData(std::list<std::shared_ptr<B1SECS2Data> >* data)
{
    /* Define Report Acknowledge (DRA)
    <DRACK>
    */
    addData(_drAck, data);
    return true;
}
