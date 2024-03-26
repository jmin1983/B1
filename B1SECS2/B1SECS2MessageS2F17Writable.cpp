//
// B1SECS2MessageS2F17Writable.cpp
//
// Library: B1SECS2
// Package: S2
// Module:  B1SECS2
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1SECS2.h"
#include "B1SECS2MessageS2F17Writable.h"

using namespace BnD;

B1SECS2MessageS2F17Writable::B1SECS2MessageS2F17Writable()
    : B1SECS2MessageWritable(true, 2, 17, "sendMessageS2F17")
{
}

B1SECS2MessageS2F17Writable::~B1SECS2MessageS2F17Writable()
{
}

B1String B1SECS2MessageS2F17Writable::fullName()
{
    return "Date and Time Data Request (DTR)";
}

bool B1SECS2MessageS2F17Writable::implWriteData(std::list<std::shared_ptr<B1SECS2Data> >* data)
{
    /* Date and Time Data Request (DTR)
    Header Only
    */
    return true;
}
