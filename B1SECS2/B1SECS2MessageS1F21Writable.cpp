//
// B1SECS2MessageS1F21Writable.cpp
//
// Library: B1SECS2
// Package: S1
// Module:  B1SECS2
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1SECS2.h"
#include "B1SECS2MessageS1F21Writable.h"

using namespace BnD;

B1SECS2MessageS1F21Writable::B1SECS2MessageS1F21Writable(const std::list<B1SECS2DataVID>& vIDs)
    : B1SECS2MessageWritable(false, 1, 21, "sendMessageS1F21")
    , _vIDs(vIDs)
{
}

B1SECS2MessageS1F21Writable::~B1SECS2MessageS1F21Writable()
{
}

B1String B1SECS2MessageS1F21Writable::fullName()
{
    return "Data Variable Namelist Request (DVNR)";
}

bool B1SECS2MessageS1F21Writable::implWriteData(std::list<std::shared_ptr<B1SECS2Data> >* data)
{
    /* Data Variable Namelist Request (DVNR)
    <L n
        <VID>
    >
    */
    addList(_vIDs.size(), data);
    for (const auto& vID : _vIDs) {
        addData(vID, data);
    }
    return true;
}
