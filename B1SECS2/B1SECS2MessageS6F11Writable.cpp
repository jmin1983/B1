//
// B1SECS2MessageS6F11Writable.cpp
//
// Library: B1SECS2
// Package: S6
// Module:  B1SECS2
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1SECS2.h"
#include "B1SECS2MessageS6F11Writable.h"

using namespace BnD;

B1SECS2MessageS6F11Writable::B1SECS2MessageS6F11Writable(const B1SECS2DataDATAID& dataID, const B1SECS2DataCEID& ceID, const std::map<B1SECS2DataRPTID, std::list<B1SECS2DataV> >& reportsData)
    : B1SECS2MessageWritable(true, 6, 11, "sendMessageS6F11")
    , _dataID(dataID)
    , _ceID(ceID)
    , _reportsData(reportsData)
{
}

B1SECS2MessageS6F11Writable::~B1SECS2MessageS6F11Writable()
{
}

B1String B1SECS2MessageS6F11Writable::fullName()
{
    return "Event Report Send (ERS)";
}

bool B1SECS2MessageS6F11Writable::implWriteData(std::list<std::shared_ptr<B1SECS2Data> >* data)
{
    /* Event Report Send (ERS)
    W
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
        const std::list<B1SECS2DataV>& vs = reportsDataPair.second;
        addList(vs.size(), data);
        for (const auto& v : vs) {
            addData(v, data);
            if (v->isFormatList()) {
                for (auto val : v._list) {
                    addData(val, data);
                }
            }
        }
    }
    return true;
}
