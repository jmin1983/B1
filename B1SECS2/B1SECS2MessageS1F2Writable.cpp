//
// B1SECS2MessageS1F2Writable.cpp
//
// Library: B1SECS2
// Package: S1
// Module:  B1SECS2
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1SECS2.h"
#include "B1SECS2MessageS1F2Writable.h"

using namespace BnD;

B1SECS2MessageS1F2Writable::B1SECS2MessageS1F2Writable()
    : B1SECS2MessageWritable(false, 1, 2, "sendMessageS1F2")
    , _mdln(NULL)
    , _softRev(NULL)
{
}

B1SECS2MessageS1F2Writable::B1SECS2MessageS1F2Writable(const B1SECS2DataMDLN& mdln, const B1SECS2DataSOFTREV& softRev)
    : B1SECS2MessageWritable(false, 1, 2, "sendMessageS1F2")
    , _mdln(&mdln)
    , _softRev(&softRev)
{
}

B1SECS2MessageS1F2Writable::~B1SECS2MessageS1F2Writable()
{
}

B1String B1SECS2MessageS1F2Writable::fullName()
{
    return _mdln ? "ON-LINE Data (D) E->H" : "ON-LINE Data (D) H->E";
}

bool B1SECS2MessageS1F2Writable::implWriteData(std::list<std::shared_ptr<B1SECS2Data> >* data)
{
    if (_mdln && _softRev) {
        /* ON-LINE Data (D) E->H
        <L 2
            <MDLN>
            <SOFTREV>
        >
        */
        addList(2, data);
        addData(*_mdln, data);
        addData(*_softRev, data);
    }
    else {
        /* ON-LINE Data (D) H->E
        <L 0
        >
        */
        addList(0, data);
    }
    return true;
}
