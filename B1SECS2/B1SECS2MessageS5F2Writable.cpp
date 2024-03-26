//
// B1SECS2MessageS5F2Writable.cpp
//
// Library: B1SECS2
// Package: S5
// Module:  B1SECS2
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1SECS2.h"
#include "B1SECS2MessageS5F2Writable.h"

using namespace BnD;

B1SECS2MessageS5F2Writable::B1SECS2MessageS5F2Writable(const B1SECS2DataACKC5& ackc5)
    : B1SECS2MessageWritable(false, 5, 2, "sendMessageS5F2")
    , _ackc5(ackc5)
{
}

B1SECS2MessageS5F2Writable::~B1SECS2MessageS5F2Writable()
{
}

B1String B1SECS2MessageS5F2Writable::fullName()
{
    return "Alarm Report Acknowledge (ARA)"; 
}

bool B1SECS2MessageS5F2Writable::implWriteData(std::list<std::shared_ptr<B1SECS2Data> >* data)
{
    /* Alarm Report Acknowledge (ARA)
    <ACKC5>
    */
    addData(_ackc5, data);
    return true;
}
