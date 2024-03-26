//
// B1SECS2MessageS1F14Writable.cpp
//
// Library: B1SECS2
// Package: S1
// Module:  B1SECS2
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1SECS2.h"
#include "B1SECS2MessageS1F14Writable.h"

using namespace BnD;

B1SECS2MessageS1F14Writable::B1SECS2MessageS1F14Writable(const B1SECS2DataCOMMACK& commAck, const B1SECS2DataMDLN* mdln, const B1SECS2DataSOFTREV* softRev)
    : B1SECS2MessageWritable(false, 1, 14, "sendMessageS1F14")
    , _commAck(commAck)
    , _mdln(mdln)
    , _softRev(softRev)
{
}

B1SECS2MessageS1F14Writable::~B1SECS2MessageS1F14Writable()
{
}

B1String B1SECS2MessageS1F14Writable::fullName()
{
    return isSentFromHost() ? "Establish Communications Request Acknowledge (CRA) H -> E" : "Establish Communications Request Acknowledge (CRA) E -> H";
}

bool B1SECS2MessageS1F14Writable::implWriteData(std::list<std::shared_ptr<B1SECS2Data> >* data)
{
    if (isSentFromHost()) {
        /* Establish Communications Request Acknowledge (CRA) H -> E
        <L 2
            < COMMACK >
            <L 0>
        >
        >*/
        addList(2, data);
        addData(_commAck, data);
        addList(0, data);
    }
    else {
        /* Establish Communications Request Acknowledge (CRA) E -> H
        <L 2
            <COMMACK>
            <L 2
                <MDLN>
                <SOFTREV>
            >
        >
        */
        addList(2, data);
        addData(_commAck, data);
        addList(2, data);
        addData(*_mdln, data);
        addData(*_softRev, data);
    }
    return true;
}
