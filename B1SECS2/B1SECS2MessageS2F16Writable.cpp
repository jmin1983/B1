//
// B1SECS2MessageS2F16Writable.cpp
//
// Library: B1SECS2
// Package: S2
// Module:  B1SECS2
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1SECS2.h"
#include "B1SECS2MessageS2F16Writable.h"

using namespace BnD;

B1SECS2MessageS2F16Writable::B1SECS2MessageS2F16Writable(const B1SECS2DataEAC& eac)
    : B1SECS2MessageWritable(false, 2, 16, "sendMessageS2F16")
    , _eac(eac)
{
}

B1SECS2MessageS2F16Writable::~B1SECS2MessageS2F16Writable()
{
}

B1String B1SECS2MessageS2F16Writable::fullName()
{
    return "New Equipment Constant Acknowledge (ECA)";
}

bool B1SECS2MessageS2F16Writable::implWriteData(std::list<std::shared_ptr<B1SECS2Data> >* data)
{
    /* New Equipment Constant Acknowledge (ECA)
    <EAC>
    */
    addData(_eac, data);
    return true;
}
