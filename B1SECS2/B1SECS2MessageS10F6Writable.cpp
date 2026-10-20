//
// B1SECS2MessageS10F6Writable.cpp
//
// Library: B1SECS2
// Package: S10
// Module:  B1SECS2
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1SECS2.h"
#include "B1SECS2MessageS10F6Writable.h"

using namespace BnD;

B1SECS2MessageS10F6Writable::B1SECS2MessageS10F6Writable(const B1SECS2DataACKC10& ackc10)
    : B1SECS2MessageWritable(false, 10, 6, "sendMessageS10F6")
    , _ackc10(ackc10)
{
}

B1SECS2MessageS10F6Writable::~B1SECS2MessageS10F6Writable()
{
}

B1String B1SECS2MessageS10F6Writable::fullName()
{
    return "Terminal Display, Multi-Block Acknowledge (VMA)";
}

bool B1SECS2MessageS10F6Writable::implWriteData(std::list<std::shared_ptr<B1SECS2Data> >* data)
{
    /* Terminal Display, Multi-Block Acknowledge (VMA)
    <ACKC10>
    */
    addData(_ackc10, data);
    return true;
}
