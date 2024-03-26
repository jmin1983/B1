//
// B1SECS2MessageS1F22Writable.cpp
//
// Library: B1SECS2
// Package: S1
// Module:  B1SECS2
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1SECS2.h"
#include "B1SECS2MessageS1F22Writable.h"

using namespace BnD;

B1SECS2MessageS1F22Writable::B1SECS2MessageS1F22Writable(const std::map<B1SECS2DataVID, std::pair<B1SECS2DataDVVALNAME, B1SECS2DataUNITS> >& vNames)
    : B1SECS2MessageWritable(false, 1, 22, "sendMessageS1F22")
    , _vNames(vNames)
{
}

B1SECS2MessageS1F22Writable::~B1SECS2MessageS1F22Writable()
{
}

B1String B1SECS2MessageS1F22Writable::fullName()
{
    return "Data Variable Namelist Reply (DVN)";
}

bool B1SECS2MessageS1F22Writable::implWriteData(std::list<std::shared_ptr<B1SECS2Data> >* data)
{
    /* Data Variable Namelist Reply (DVN)
    <L n
        <L 3
            <VID>
            <DVVALNAME>
            <UNITS>
        >
    >
    */
    addList(_vNames.size(), data);
    for (const auto& vNamesPair : _vNames) {
        addList(3, data);
        addData(vNamesPair.first, data);
        addData(vNamesPair.second.first, data);
        addData(vNamesPair.second.second, data);
    }
    return true;
}
