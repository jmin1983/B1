//
// B1SECS2MessageS2F42Writable.cpp
//
// Library: B1SECS2
// Package: S2
// Module:  B1SECS2
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1SECS2.h"
#include "B1SECS2MessageS2F42Writable.h"

using namespace BnD;

B1SECS2MessageS2F42Writable::B1SECS2MessageS2F42Writable(const B1SECS2DataHCACK& hcAck, const std::map<B1SECS2DataCPNAME, B1SECS2DataCPACK>& cps)
    : B1SECS2MessageWritable(false, 2, 42, "sendMessageS2F42")
    , _hcAck(hcAck)
    , _cps(cps)
{
}

B1SECS2MessageS2F42Writable::~B1SECS2MessageS2F42Writable()
{
}

B1String B1SECS2MessageS2F42Writable::fullName()
{
    return "Host Command Send Ack (S2F42)";
}

bool B1SECS2MessageS2F42Writable::implWriteData(std::list<std::shared_ptr<B1SECS2Data> >* data)
{
    /* Host Command Send Ack (S2F42)
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
