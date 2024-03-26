//
// B1SECS2MessageS6F16Writable.cpp
//
// Library: B1SECS2
// Package: S6
// Module:  B1SECS2
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1SECS2.h"
#include "B1SECS2MessageS6F16Writable.h"

using namespace BnD;

B1SECS2MessageS6F16Writable::B1SECS2MessageS6F16Writable(const B1SECS2DataDATAID& dataID, const B1SECS2DataCEID& ceID, const std::map<B1SECS2DataRPTID, std::list<B1SECS2DataV>>& reportsData)
    : B1SECS2MessageWritable(false, 6, 16, "sendMessageS6F16")
    , _dataID(dataID)
    , _ceID(ceID)
    , _reportsData(reportsData)
{
}

B1SECS2MessageS6F16Writable::~B1SECS2MessageS6F16Writable()
{
}

B1String B1SECS2MessageS6F16Writable::fullName()
{
    return "Event Report Data (ERD)";
}

bool B1SECS2MessageS6F16Writable::implWriteData(std::list<std::shared_ptr<B1SECS2Data> >* data)
{
    /* Event Report Data (ERD)
    <L 3
        <DATAID>
        <CEID>
        <L a
            <L 2
                <RPTID>
                <L b
                    <V>
                >
            >
        >
    >
    */
    addList(3, data);
    addData(_dataID, data);
    addData(_ceID, data);
    addList(_reportsData.size(), data);
    for (const auto& reportsDataPair : _reportsData) {
        addList(2, data);
        addData(reportsDataPair.first, data);
        const auto& vs = reportsDataPair.second;
        addList(vs.size(), data);
        for (const auto& v : vs) {
            addData(v, data);
            if (v->isFormatList()) {
                addData(v._list, data);
            }
        }
    }
    return true;
}
