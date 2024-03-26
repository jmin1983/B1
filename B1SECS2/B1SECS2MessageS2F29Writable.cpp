//
// B1SECS2MessageS2F29Writable.cpp
//
// Library: B1SECS2
// Package: S2
// Module:  B1SECS2
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1SECS2.h"
#include "B1SECS2MessageS2F29Writable.h"

using namespace BnD;

B1SECS2MessageS2F29Writable::B1SECS2MessageS2F29Writable(const std::list<B1SECS2DataECID>& ecIDs)
    : B1SECS2MessageWritable(true, 2, 29, "sendMessageS2F29")
    , _ecIDs(ecIDs)
{
}

B1SECS2MessageS2F29Writable::~B1SECS2MessageS2F29Writable()
{
}

B1String B1SECS2MessageS2F29Writable::fullName()
{
    return "Equipment Constant Namelist Request (ECNR)";
}

bool B1SECS2MessageS2F29Writable::implWriteData(std::list<std::shared_ptr<B1SECS2Data> >* data)
{
    /* Equipment Constant Namelist (ECN)
    <L n
        <ECID>
    >
    */
    addList(_ecIDs.size(), data);
    for (const auto& ecID : _ecIDs) {
        addData(ecID, data);
    }
    return true;
}
