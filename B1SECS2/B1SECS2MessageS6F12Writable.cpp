//
// B1SECS2MessageS6F12Writable.cpp
//
// Library: B1SECS2
// Package: S6
// Module:  B1SECS2
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1SECS2.h"
#include "B1SECS2MessageS6F12Writable.h"

using namespace BnD;

B1SECS2MessageS6F12Writable::B1SECS2MessageS6F12Writable(const B1SECS2DataACKC6& ackc6)
    : B1SECS2MessageWritable(false, 6, 12, "sendMessageS6F12")
    , _ackc6(ackc6)
{
}

B1SECS2MessageS6F12Writable::~B1SECS2MessageS6F12Writable()
{
}

B1String B1SECS2MessageS6F12Writable::fullName()
{
    return "Event Report Acknowledge (ERA)";
}

bool B1SECS2MessageS6F12Writable::implWriteData(std::list<std::shared_ptr<B1SECS2Data> >* data)
{
    /* Event Report Acknowledge (ERA)
    <ACKC6>
    */
    addData(_ackc6, data);
    return true;
}
