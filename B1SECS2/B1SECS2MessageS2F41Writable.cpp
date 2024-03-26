//
// B1SECS2MessageS2F41Writable.cpp
//
// Library: B1SECS2
// Package: S2
// Module:  B1SECS2
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1SECS2.h"
#include "B1SECS2MessageS2F41Writable.h"

using namespace BnD;

B1SECS2MessageS2F41Writable::B1SECS2MessageS2F41Writable(const B1SECS2DataRCMD& rcmd, const std::map<B1SECS2DataCPNAME, B1SECS2DataCPVAL>& cps)
    : B1SECS2MessageWritable(true, 2, 41, "sendMessageS2F41")
    , _rcmd(rcmd)
    , _cps(cps)
{
}

B1SECS2MessageS2F41Writable::~B1SECS2MessageS2F41Writable()
{
}

B1String B1SECS2MessageS2F41Writable::fullName()
{
    return "Host Command Send (HCS)";
}

bool B1SECS2MessageS2F41Writable::implWriteData(std::list<std::shared_ptr<B1SECS2Data> >* data)
{
    /* Host Command Send (HCS)
    W
    <L 2
        <RCMD>
        <L n
            <L 2
                <CPNAME>
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
