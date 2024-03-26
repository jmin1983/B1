//
// B1SECS2MessageS6F15Writable.cpp
//
// Library: B1SECS2
// Package: S6
// Module:  B1SECS2
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1SECS2.h"
#include "B1SECS2MessageS6F15Writable.h"

using namespace BnD;

B1SECS2MessageS6F15Writable::B1SECS2MessageS6F15Writable(const B1SECS2DataCEID& ceID)
    : B1SECS2MessageWritable(true, 6, 15, "sendMessageS6F15")
    , _ceID(ceID)
{
}

B1SECS2MessageS6F15Writable::~B1SECS2MessageS6F15Writable()
{
}

B1String B1SECS2MessageS6F15Writable::fullName()
{
    return "Event Report Request (ERR)";
}

bool B1SECS2MessageS6F15Writable::implWriteData(std::list<std::shared_ptr<B1SECS2Data> >* data)
{
    /* Event Report Data (ERD)
    <CEID>
    */
    addData(_ceID, data);
    return true;
}
