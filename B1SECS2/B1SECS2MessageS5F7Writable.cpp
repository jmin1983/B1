//
// B1SECS2MessageS5F7Writable.cpp
//
// Library: B1SECS2
// Package: S5
// Module:  B1SECS2
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1SECS2.h"
#include "B1SECS2MessageS5F7Writable.h"

using namespace BnD;

B1SECS2MessageS5F7Writable::B1SECS2MessageS5F7Writable()
    : B1SECS2MessageWritable(true, 5, 7, "sendMessageS5F7")
{
}

B1SECS2MessageS5F7Writable::~B1SECS2MessageS5F7Writable()
{
}

B1String B1SECS2MessageS5F7Writable::fullName()
{
    return "List Enabled Alarm Request (LEAR)";
}

bool B1SECS2MessageS5F7Writable::implWriteData(std::list<std::shared_ptr<B1SECS2Data> >* data)
{
    /* List Enabled Alarm Request (LEAR)
    Header Only
    */
    return true;
}
