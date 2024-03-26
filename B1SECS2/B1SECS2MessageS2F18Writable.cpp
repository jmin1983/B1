//
// B1SECS2MessageS2F18Writable.cpp
//
// Library: B1SECS2
// Package: S2
// Module:  B1SECS2
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1SECS2.h"
#include "B1SECS2MessageS2F18Writable.h"

using namespace BnD;

B1SECS2MessageS2F18Writable::B1SECS2MessageS2F18Writable(const B1SECS2DataTIME& time)
    : B1SECS2MessageWritable(false, 2, 18, "sendMessageS2F18")
    , _time(time)
{
}

B1SECS2MessageS2F18Writable::~B1SECS2MessageS2F18Writable()
{
}

B1String B1SECS2MessageS2F18Writable::fullName()
{
    return "Date and Time Data (DTD)";
}

bool B1SECS2MessageS2F18Writable::implWriteData(std::list<std::shared_ptr<B1SECS2Data> >* data)
{
    /* Date and Time Data (DTD)
    <TIME>
    */
    addData(_time, data);
    return true;
}
