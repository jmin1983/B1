//
// B1SECS2MessageS1F1Writable.cpp
//
// Library: B1SECS2
// Package: S1
// Module:  B1SECS2
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1SECS2.h"
#include "B1SECS2MessageS1F1Writable.h"

using namespace BnD;

B1SECS2MessageS1F1Writable::B1SECS2MessageS1F1Writable()
    : B1SECS2MessageWritable(true, 1, 1, "sendMessageS1F1")
{
}

B1SECS2MessageS1F1Writable::~B1SECS2MessageS1F1Writable()
{
}

B1String B1SECS2MessageS1F1Writable::fullName()
{
    return "Are You There Request (R)";
}

bool B1SECS2MessageS1F1Writable::implWriteData(std::list<std::shared_ptr<B1SECS2Data> >* data)
{
    /* Are You There Request (R)
    */
    return true;
}
