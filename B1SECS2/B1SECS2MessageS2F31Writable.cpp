//
// B1SECS2MessageS2F31Writable.cpp
//
// Library: B1SECS2
// Package: S2
// Module:  B1SECS2
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1SECS2.h"
#include "B1SECS2MessageS2F31Writable.h"

using namespace BnD;

B1SECS2MessageS2F31Writable::B1SECS2MessageS2F31Writable(const B1SECS2DataTIME& time)
    : B1SECS2MessageWritable(true, 2, 31, "sendMessageS2F31")
    , _time(time)
{
}

B1SECS2MessageS2F31Writable::~B1SECS2MessageS2F31Writable()
{
}

B1String B1SECS2MessageS2F31Writable::fullName()
{
    return "Date and Time Set Request (DTS)";
}

bool B1SECS2MessageS2F31Writable::implWriteData(std::list<std::shared_ptr<B1SECS2Data> >* data)
{
    /* Date and Time Set (DTS)
    <TIME>
    */
    addData(_time, data);
    return true;
}
