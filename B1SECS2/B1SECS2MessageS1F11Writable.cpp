//
// B1SECS2MessageS1F11Writable.cpp
//
// Library: B1SECS2
// Package: S1
// Module:  B1SECS2
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1SECS2.h"
#include "B1SECS2MessageS1F11Writable.h"

using namespace BnD;

B1SECS2MessageS1F11Writable::B1SECS2MessageS1F11Writable(const std::list<B1SECS2DataSVID>& svIDs)
    : B1SECS2MessageWritable(true, 1, 11, "sendMessageS1F11")
    , _svIDs(svIDs)
{
}

B1SECS2MessageS1F11Writable::~B1SECS2MessageS1F11Writable()
{
}

B1String B1SECS2MessageS1F11Writable::fullName()
{
    return "Status Variable Namelist Request (SVNR)";
}

bool B1SECS2MessageS1F11Writable::implWriteData(std::list<std::shared_ptr<B1SECS2Data> >* data)
{
    /* Status Variable Namelist Request (SVNR)
    <L n
        <SVID>
    >
    */
    addList(_svIDs.size(), data);
    for (const auto& svID : _svIDs) {
        addData(svID, data);
    }
    return true;
}
