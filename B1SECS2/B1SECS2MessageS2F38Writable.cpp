//
// B1SECS2MessageS2F38Writable.cpp
//
// Library: B1SECS2
// Package: S2
// Module:  B1SECS2
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1SECS2.h"
#include "B1SECS2MessageS2F38Writable.h"

using namespace BnD;

B1SECS2MessageS2F38Writable::B1SECS2MessageS2F38Writable(const B1SECS2DataERACK& erAck)
    : B1SECS2MessageWritable(false, 2, 38, "sendMessageS2F38")
    , _erAck(erAck)
{
}

B1SECS2MessageS2F38Writable::~B1SECS2MessageS2F38Writable()
{
}

B1String B1SECS2MessageS2F38Writable::fullName()
{
    return "Enable/Disable Event Report Acknowledge (EERA)";
}

bool B1SECS2MessageS2F38Writable::implWriteData(std::list<std::shared_ptr<B1SECS2Data> >* data)
{
    /* Enable/Disable Event Report Acknowledge (EERA)
    <ERACK>
    */
    addData(_erAck, data);
    return true;
}
