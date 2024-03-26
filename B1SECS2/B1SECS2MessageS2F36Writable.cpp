//
// B1SECS2MessageS2F36Writable.cpp
//
// Library: B1SECS2
// Package: S2
// Module:  B1SECS2
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1SECS2.h"
#include "B1SECS2MessageS2F36Writable.h"

using namespace BnD;

B1SECS2MessageS2F36Writable::B1SECS2MessageS2F36Writable(const B1SECS2DataLRACK& lrAck)
    : B1SECS2MessageWritable(false, 2, 36, "sendMessageS2F36")
    , _lrAck(lrAck)
{
}

B1SECS2MessageS2F36Writable::~B1SECS2MessageS2F36Writable()
{
}

B1String B1SECS2MessageS2F36Writable::fullName()
{
    return "Link Event Report Acknowledge (LERA)";
}

bool B1SECS2MessageS2F36Writable::implWriteData(std::list<std::shared_ptr<B1SECS2Data> >* data)
{
    /* Link Event Report Acknowledge (LERA)
    <LRACK>
    */
    addData(_lrAck, data);
    return true;
}
