//
// B1SECS2MessageS1F12Writable.cpp
//
// Library: B1SECS2
// Package: S1
// Module:  B1SECS2
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1SECS2.h"
#include "B1SECS2MessageS1F12Writable.h"

using namespace BnD;

B1SECS2MessageS1F12Writable::B1SECS2MessageS1F12Writable(const std::map<B1SECS2DataSVID, std::pair<B1SECS2DataSVNAME, B1SECS2DataUNITS> >& svs)
    : B1SECS2MessageWritable(false, 1, 12, "sendMessageS1F12")
    , _svs(svs)
{
}

B1SECS2MessageS1F12Writable::~B1SECS2MessageS1F12Writable()
{
}

B1String B1SECS2MessageS1F12Writable::fullName()
{
    return "Status Variable Namelist Reply (SVNRR)";
}

bool B1SECS2MessageS1F12Writable::implWriteData(std::list<std::shared_ptr<B1SECS2Data> >* data)
{
    /* Status Variable Namelist Reply (SVNRR)
    <L n
        <L 3
            <SVID>
            <SVNAME>
            <UNITS>
        >
    >
    */
    addList(_svs.size(), data);
    for (const auto& svsPair : _svs) {
        addList(3, data);
        addData(svsPair.first, data);
        addData(svsPair.second.first, data);
        addData(svsPair.second.second, data);
    }
    return true;
}
