//
// B1SECS2MessageS6F19Writable.cpp
//
// Library: B1SECS2
// Package: S6
// Module:  B1SECS2
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1SECS2.h"
#include "B1SECS2MessageS6F19Writable.h"

using namespace BnD;

B1SECS2MessageS6F19Writable::B1SECS2MessageS6F19Writable(const B1SECS2DataRPTID& reportID)
    : B1SECS2MessageWritable(true, 6, 19, "sendMessageS6F19")
    , _reportID(reportID)
{
}

B1SECS2MessageS6F19Writable::~B1SECS2MessageS6F19Writable()
{
}

B1String B1SECS2MessageS6F19Writable::fullName()
{
    return "Individual Report Request(IRR)";
}

bool B1SECS2MessageS6F19Writable::implWriteData(std::list<std::shared_ptr<B1SECS2Data> >* data)
{
    /* Individual Report Request (IRR)
    <RPTID>
    */
    addData(_reportID, data);
    return true;
}
