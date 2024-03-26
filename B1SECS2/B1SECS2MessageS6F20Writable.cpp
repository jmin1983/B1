//
// B1SECS2MessageS6F20Writable.cpp
//
// Library: B1SECS2
// Package: S6
// Module:  B1SECS2
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1SECS2.h"
#include "B1SECS2MessageS6F20Writable.h"

using namespace BnD;

B1SECS2MessageS6F20Writable::B1SECS2MessageS6F20Writable(const std::list<B1SECS2DataV>& vs)
    : B1SECS2MessageWritable(false, 6, 20, "sendMessageS6F20")
    , _vs(vs)
{
}

B1SECS2MessageS6F20Writable::~B1SECS2MessageS6F20Writable()
{
}

B1String B1SECS2MessageS6F20Writable::fullName()
{
    return "Individual Report Data (IRD)";
}

bool B1SECS2MessageS6F20Writable::implWriteData(std::list<std::shared_ptr<B1SECS2Data> >* data)
{
    /* Individual Report Data (IRD)
    <L a
        <V> ALL, n
    >
    */
    addList(_vs.size(), data);
    for (const auto& v : _vs) {
        addData(v, data);
        if (v->isFormatList()) {
            addData(v._list, data);
        }
    }
    return true;
}
