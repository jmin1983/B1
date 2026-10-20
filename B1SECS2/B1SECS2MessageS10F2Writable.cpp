//
// B1SECS2MessageS10F2Writable.cpp
//
// Library: B1SECS2
// Package: S10
// Module:  B1SECS2
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1SECS2.h"
#include "B1SECS2MessageS10F2Writable.h"

using namespace BnD;

B1SECS2MessageS10F2Writable::B1SECS2MessageS10F2Writable(const B1SECS2DataACKC10& ackc10)
    : B1SECS2MessageWritable(false, 10, 2, "sendMessageS10F2")
    , _ackc10(ackc10)
{
}

B1SECS2MessageS10F2Writable::~B1SECS2MessageS10F2Writable()
{
}

B1String B1SECS2MessageS10F2Writable::fullName()
{
    return "Terminal Request Acknowledge (TRA)";
}

bool B1SECS2MessageS10F2Writable::implWriteData(std::list<std::shared_ptr<B1SECS2Data> >* data)
{
    /* Terminal Request Acknowledge (TRA)
    <ACKC10>
    */
    addData(_ackc10, data);
    return true;
}
