//
// B1SECS2MessageS2F15Writable.cpp
//
// Library: B1SECS2
// Package: S2
// Module:  B1SECS2
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1SECS2.h"
#include "B1SECS2MessageS2F15Writable.h"

using namespace BnD;

B1SECS2MessageS2F15Writable::B1SECS2MessageS2F15Writable(const std::map<B1SECS2DataECID, B1SECS2DataECV>& ecvs)
    : B1SECS2MessageWritable(true, 2, 15, "sendMessageS2F15")
    , _ecvs(ecvs)
{
}

B1SECS2MessageS2F15Writable::~B1SECS2MessageS2F15Writable()
{
}

B1String B1SECS2MessageS2F15Writable::fullName()
{
    return "New Equipment Constant Send (ECS)";
}

bool B1SECS2MessageS2F15Writable::implWriteData(std::list<std::shared_ptr<B1SECS2Data> >* data)
{
    /* New Equipment Constant Send (ECS)
    <L a
        <L 2
            <ECID>
            <ECV>
        >
    >
    */
    addList(_ecvs.size(), data);
    for (const auto& ecvsPair : _ecvs) {
        addList(2, data);
        addData(ecvsPair.first, data);
        addData(ecvsPair.second, data);
    }
    return true;
}
