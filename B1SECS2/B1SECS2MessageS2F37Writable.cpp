//
// B1SECS2MessageS2F37Writable.cpp
//
// Library: B1SECS2
// Package: S2
// Module:  B1SECS2
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1SECS2.h"
#include "B1SECS2MessageS2F37Writable.h"

using namespace BnD;

B1SECS2MessageS2F37Writable::B1SECS2MessageS2F37Writable(const B1SECS2DataCEED& ceed, const std::list<B1SECS2DataCEID>& ceIDs)
    : B1SECS2MessageWritable(true, 2, 37, "sendMessageS2F37")
    , _ceed(ceed)
    , _ceIDs(ceIDs)
{
}

B1SECS2MessageS2F37Writable::~B1SECS2MessageS2F37Writable()
{
}

B1String B1SECS2MessageS2F37Writable::fullName()
{
    return "Enable/Disable Event Report (EDER)";
}

bool B1SECS2MessageS2F37Writable::implWriteData(std::list<std::shared_ptr<B1SECS2Data> >* data)
{
    /* Enable/Disable Event Report (EDER)
    <L 2
        <CEED>
        <L n
            <CEID>
        >
    >
    */
    addList(2, data);
    addData(_ceed, data);
    addList(_ceIDs.size(), data);
    for (const auto& ceID : _ceIDs) {
        addData(ceID, data);
    }
    return true;
}
