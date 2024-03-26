//
// B1SECS2MessageS1F15Writable.cpp
//
// Library: B1SECS2
// Package: S1
// Module:  B1SECS2
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1SECS2.h"
#include "B1SECS2MessageS1F15Writable.h"

using namespace BnD;

B1SECS2MessageS1F15Writable::B1SECS2MessageS1F15Writable()
    : B1SECS2MessageWritable(true, 1, 15, "sendMessageS1F15")
{
}

B1SECS2MessageS1F15Writable::~B1SECS2MessageS1F15Writable()
{
}

B1String B1SECS2MessageS1F15Writable::fullName()
{
    return "Request OFF-LINE (ROFL)";
}

bool B1SECS2MessageS1F15Writable::implWriteData(std::list<std::shared_ptr<B1SECS2Data> >* data)
{
    /* Request OFF-LINE (ROFL)
    */
    return true;
}
