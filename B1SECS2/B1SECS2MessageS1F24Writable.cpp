//
// B1SECS2MessageS1F24Writable.cpp
//
// Library: B1SECS2
// Package: S1
// Module:  B1SECS2
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1SECS2.h"
#include "B1SECS2MessageS1F24Writable.h"

using namespace BnD;

B1SECS2MessageS1F24Writable::B1SECS2MessageS1F24Writable(const std::map<B1SECS2DataCEID, std::pair<B1SECS2DataCENAME, std::list<B1SECS2DataVID> > >& ceNames)
    : B1SECS2MessageWritable(false, 1, 24, "sendMessageS1F24")
    , _ceNames(ceNames)
{
}

B1SECS2MessageS1F24Writable::~B1SECS2MessageS1F24Writable()
{
}

B1String B1SECS2MessageS1F24Writable::fullName()
{
    return "Collection Event Namelist (CEN)";
}

bool B1SECS2MessageS1F24Writable::implWriteData(std::list<std::shared_ptr<B1SECS2Data> >* data)
{
    /* Collection Event Namelist (CEN)
    <L n
        <L 3
            <CEID>
            <CENAME>
            <L a
                <VID>
            >
        >
    >
    */
    addList(_ceNames.size(), data);
    for (const auto& ceNamesPair : _ceNames) {
        addList(3, data);
        addData(ceNamesPair.first, data);
        addData(ceNamesPair.second.first, data);
        const std::list<B1SECS2DataVID>& vIDs = ceNamesPair.second.second;
        addList(vIDs.size(), data);
        for (const auto& vID : vIDs) {
            addData(vID, data);
        }
    }
    return true;
}
