//
// B1SECS2MessageS2F49Writable.cpp
//
// Library: B1SECS2
// Package: S2
// Module:  B1SECS2
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1SECS2.h"
#include "B1SECS2MessageS2F49Writable.h"

using namespace BnD;

B1SECS2MessageS2F49Writable::B1SECS2MessageS2F49Writable(const B1SECS2DataDATAID& dataID, const B1SECS2DataOBJSPEC& objSpec, const B1SECS2DataRCMD& rcmd,
                                                     const std::map<B1SECS2DataCPNAME, B1SECS2DataCEPVAL>& ceps)
    : B1SECS2MessageWritable(true, 2, 49, "sendMessageS2F49")
    , _dataID(dataID)
    , _objSpec(objSpec)
    , _rcmd(rcmd)
    , _ceps(ceps)
{
}

B1SECS2MessageS2F49Writable::~B1SECS2MessageS2F49Writable()
{
}

B1String B1SECS2MessageS2F49Writable::fullName()
{
    return "Enhanced Remote Command Send (S2F49)";
}

bool B1SECS2MessageS2F49Writable::implWriteData(std::list<std::shared_ptr<B1SECS2Data> >* data)
{
    /* Enhanced Remote Command Send
    W
    <L 4
        <DATAID>
        <OBJSPEC>
        <RCMD>
        <L n
            <L 2
                <CPNAME>
                <CEPVAL>
            >
        >
    >
    */
    addList(4, data);
    addData(_dataID, data);
    addData(_objSpec, data);
    addData(_rcmd, data);
    addList(_ceps.size(), data);
    for (const auto& cepsPair : _ceps) {
        addList(2, data);
        addData(cepsPair.first, data);
        const B1SECS2DataCEPVAL& cepVAL = cepsPair.second;
        addData(cepVAL, data);
        if (cepVAL->isFormatList()) {
            addData(cepVAL._list, data);
        }
    }
    return true;
}
