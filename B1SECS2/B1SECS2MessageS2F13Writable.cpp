//
// B1SECS2MessageS2F13Writable.cpp
//
// Library: B1SECS2
// Package: S2
// Module:  B1SECS2
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1SECS2.h"
#include "B1SECS2MessageS2F13Writable.h"

using namespace BnD;

B1SECS2MessageS2F13Writable::B1SECS2MessageS2F13Writable(const std::list<B1SECS2DataECID>& ecIDs)
    : B1SECS2MessageWritable(true, 2, 13, "sendMessageS2F13")
    , _ecIDs(ecIDs)
{
}

B1SECS2MessageS2F13Writable::~B1SECS2MessageS2F13Writable()
{
}

B1String B1SECS2MessageS2F13Writable::fullName()
{
    return "Equipment Constant Data Request (ECR)";
}

bool B1SECS2MessageS2F13Writable::implWriteData(std::list<std::shared_ptr<B1SECS2Data> >* data)
{
    /* Equipment Constant Data Request (ECR)
    <L a
        <ECID>
    >
    */
    addList(_ecIDs.size(), data);
    for (const auto& ecID : _ecIDs) {
        addData(ecID, data);
    }
    return true;
}
