//
// B1SECS2MessageS64F1Writable.cpp
//
// Library: B1SECS2
// Package: Custom
// Module:  B1SECS2
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1SECS2.h"
#include "B1SECS2MessageS64F1Writable.h"

using namespace BnD;

B1SECS2MessageS64F1Writable::B1SECS2MessageS64F1Writable(const B1SECS2DataRCMD& rcmd, const std::map<B1SECS2DataCPNAME, B1SECS2DataCPVAL>& cps)
    : B1SECS2MessageWritable(true, 64, 1, "sendMessageS64F1")
    , _rcmd(rcmd)
    , _cps(cps)
{
}

B1SECS2MessageS64F1Writable::~B1SECS2MessageS64F1Writable()
{
}

B1String B1SECS2MessageS64F1Writable::fullName()
{
    return "CUSTOM: Force Install Carrier (S64F1)";
}

bool B1SECS2MessageS64F1Writable::implWriteData(std::list<std::shared_ptr<B1SECS2Data> >* data)
{
    /*  CUSTOM: Force Install Carrier (S64F1)
    W
    <L 2
        <RCMD>              //  test_carrier_installed:[CARRIERINSTALLED]
        <L n
            <L 2
                <CPNAME>    //  test_carrier_installed:[PORTID, CARRIERID]
                <CPVAL>
            >
        >
    >
    */
    addList(2, data);
    addData(_rcmd, data);
    addList(_cps.size(), data);
    for (const auto& cpsPair : _cps) {
        addList(2, data);
        addData(cpsPair.first, data);
        addData(cpsPair.second, data);
    }
    return true;
}
