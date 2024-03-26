//
// B1SECS2MessageS64F2Writable.cpp
//
// Library: B1SECS2
// Package: Custom
// Module:  B1SECS2
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1SECS2.h"
#include "B1SECS2MessageS64F2Writable.h"

using namespace BnD;

B1SECS2MessageS64F2Writable::B1SECS2MessageS64F2Writable(const B1SECS2DataHCACK& hcAck, const std::map<B1SECS2DataCPNAME, B1SECS2DataCPACK>& cps)
    : B1SECS2MessageWritable(false, 64, 2, "sendMessageS64F2")
    , _hcAck(hcAck)
    , _cps(cps)
{
}

B1SECS2MessageS64F2Writable::~B1SECS2MessageS64F2Writable()
{
}

B1String B1SECS2MessageS64F2Writable::fullName()
{
    return "CUSTOM: Force Install Carrier Ack (S64F2)";
}

bool B1SECS2MessageS64F2Writable::implWriteData(std::list<std::shared_ptr<B1SECS2Data> >* data)
{
    /*  CUSTOM: Force Install Carrier Ack (S64F2)
    <L 2
        <HCACK>
        <L n
            <L 2
                <CPNAME>
                <CPACK>
            >
        >
    >
    */
    addList(2, data);
    addData(_hcAck, data);
    addList(_cps.size(), data);
    for (const auto& cpsPair : _cps) {
        addList(2, data);
        addData(cpsPair.first, data);
        addData(cpsPair.second, data);
    }
    return true;
}
