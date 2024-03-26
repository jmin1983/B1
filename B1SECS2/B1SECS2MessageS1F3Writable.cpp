//
// B1SECS2MessageS1F3Writable.cpp
//
// Library: B1SECS2
// Package: S1
// Module:  B1SECS2
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1SECS2.h"
#include "B1SECS2MessageS1F3Writable.h"

using namespace BnD;

B1SECS2MessageS1F3Writable::B1SECS2MessageS1F3Writable(const std::list<B1SECS2DataSVID>& svIDs)
    : B1SECS2MessageWritable(true, 1, 3, "sendMessageS1F3")
    , _svIDs(svIDs)
{
}

B1SECS2MessageS1F3Writable::~B1SECS2MessageS1F3Writable()
{
}

B1String B1SECS2MessageS1F3Writable::fullName()
{
    return "Selected Equipment Status Request (SSR)";
}

bool B1SECS2MessageS1F3Writable::implWriteData(std::list<std::shared_ptr<B1SECS2Data> >* data)
{
    /* Selected Equipment Status Data (SSD)
    <L a
        <SVID>
    >
    */
    addList(_svIDs.size(), data);
    for (const auto& svID : _svIDs) {
        addData(svID, data);
    }
    return true;
}
