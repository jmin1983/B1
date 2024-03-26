//
// B1SECS2MessageS2F14Writable.cpp
//
// Library: B1SECS2
// Package: S2
// Module:  B1SECS2
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1SECS2.h"
#include "B1SECS2MessageS2F14Writable.h"

using namespace BnD;

B1SECS2MessageS2F14Writable::B1SECS2MessageS2F14Writable(const std::list<B1SECS2DataECV>& ecvs)
    : B1SECS2MessageWritable(false, 2, 14, "sendMessageS2F14")
    , _ecvs(ecvs)
{
}

B1SECS2MessageS2F14Writable::~B1SECS2MessageS2F14Writable()
{
}

B1String B1SECS2MessageS2F14Writable::fullName()
{
    return "Equipment Constant Data (ECD)";
}

bool B1SECS2MessageS2F14Writable::implWriteData(std::list<std::shared_ptr<B1SECS2Data> >* data)
{
    /* Equipment Constant Data (ECD)
    <L a
        <ECV>
    >
    */
    addList(_ecvs.size(), data);
    for (const auto& ecv : _ecvs) {
        addData(ecv, data);
    }
    return true;
}
