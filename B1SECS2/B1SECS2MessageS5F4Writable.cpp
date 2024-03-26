//
// B1SECS2MessageS5F4Writable.cpp
//
// Library: B1SECS2
// Package: S5
// Module:  B1SECS2
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1SECS2.h"
#include "B1SECS2MessageS5F4Writable.h"

using namespace BnD;

B1SECS2MessageS5F4Writable::B1SECS2MessageS5F4Writable(const B1SECS2DataACKC5& ackc5)
    : B1SECS2MessageWritable(false, 5, 4, "sendMessageS5F4")
    , _ackc5(ackc5)
{
}

B1SECS2MessageS5F4Writable::~B1SECS2MessageS5F4Writable()
{
}

B1String B1SECS2MessageS5F4Writable::fullName()
{
    return "Enable/Disable Alarm Acknowledge (EAA)";
}

bool B1SECS2MessageS5F4Writable::implWriteData(std::list<std::shared_ptr<B1SECS2Data> >* data)
{
    /* Enable/Disable Alarm Acknowledge (EAA)
    <ACKC5>
    */
    addData(_ackc5, data);
    return true;
}
