//
// B1SECS2MessageS1F23Writable.cpp
//
// Library: B1SECS2
// Package: S1
// Module:  B1SECS2
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1SECS2.h"
#include "B1SECS2MessageS1F23Writable.h"

using namespace BnD;

B1SECS2MessageS1F23Writable::B1SECS2MessageS1F23Writable(const std::list<B1SECS2DataCEID>& ceIDs)
    : B1SECS2MessageWritable(false, 1, 23, "sendMessageS1F23")
    , _ceIDs(ceIDs)
{
}

B1SECS2MessageS1F23Writable::~B1SECS2MessageS1F23Writable()
{
}

B1String B1SECS2MessageS1F23Writable::fullName()
{
    return "Collection Event Namelist Request(CENR)";
}

bool B1SECS2MessageS1F23Writable::implWriteData(std::list<std::shared_ptr<B1SECS2Data> >* data)
{
    /* Collection Event Namelist (CENR)
    <L n
        <CEID>
    >
    */
    addList(_ceIDs.size(), data);
    for (const auto& ceID : _ceIDs) {
        addData(ceID, data);
    }
    return true;
}
