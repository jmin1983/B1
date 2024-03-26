//
// B1SECS2MessageS2F35Writable.cpp
//
// Library: B1SECS2
// Package: S2
// Module:  B1SECS2
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1SECS2.h"
#include "B1SECS2MessageS2F35Writable.h"

using namespace BnD;

B1SECS2MessageS2F35Writable::B1SECS2MessageS2F35Writable(const B1SECS2DataDATAID& dataID, const std::map<B1SECS2DataCEID, std::list<B1SECS2DataRPTID> >& linkEventData)
    : B1SECS2MessageWritable(true, 2, 35, "sendMessageS2F35")
    , _dataID(dataID)
    , _linkEventData(linkEventData)
{
}

B1SECS2MessageS2F35Writable::~B1SECS2MessageS2F35Writable()
{
}

B1String B1SECS2MessageS2F35Writable::fullName()
{
    return "Link Event Report (LER)";
}

bool B1SECS2MessageS2F35Writable::implWriteData(std::list<std::shared_ptr<B1SECS2Data> >* data)
{
    /* Link Event Report (LER)
    <L 2
        <DATAID>
        <L a
            <L 2
                <CEID>
                <L b
                    <RPTID>
                >
            >
        >
    >
    */
    addList(2, data);
    addData(_dataID, data);
    addList(_linkEventData.size(), data);
    for (const auto& linkEventDataPair : _linkEventData) {
        addList(2, data);
        addData(linkEventDataPair.first, data);
        const std::list<B1SECS2DataRPTID>& rptIDs = linkEventDataPair.second;
        addList(rptIDs.size(), data);
        for (const auto& rptID : rptIDs) {
            addData(rptID, data);
        }
    }
    return true;
}
