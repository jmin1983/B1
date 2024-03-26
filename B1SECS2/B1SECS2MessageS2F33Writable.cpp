//
// B1SECS2MessageS2F33Writable.cpp
//
// Library: B1SECS2
// Package: S2
// Module:  B1SECS2
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1SECS2.h"
#include "B1SECS2MessageS2F33Writable.h"

using namespace BnD;

B1SECS2MessageS2F33Writable::B1SECS2MessageS2F33Writable(const B1SECS2DataDATAID& dataID, const std::map<B1SECS2DataRPTID, std::list<B1SECS2DataVID> >& reportData)
    : B1SECS2MessageWritable(true, 2, 33, "sendMessageS2F33")
    , _dataID(dataID)
    , _reportData(reportData)
{
}

B1SECS2MessageS2F33Writable::~B1SECS2MessageS2F33Writable()
{
}

B1String B1SECS2MessageS2F33Writable::fullName()
{
    return "Define Report (DR)";
}

bool B1SECS2MessageS2F33Writable::implWriteData(std::list<std::shared_ptr<B1SECS2Data> >* data)
{
    /* Define Report (DR)
    <L 2
        <DATAID>
        <L a
            <L 2
                <RPTID>
                <L b
                    <VID>
                >
            >
        >
    >
    */
    addList(2, data);
    addData(_dataID, data);
    addList(_reportData.size(), data);
    for (const auto& reportData : _reportData) {
        addList(2, data);
        addData(reportData.first, data);
        const std::list<B1SECS2DataVID>& vIDs = reportData.second;
        addList(vIDs.size(), data);
        for (const auto& vID : vIDs) {
            addData(vID, data);
        }
    }
    return true;
}
