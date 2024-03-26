//
// B1SECS2MessageS1F17Writable.cpp
//
// Library: B1SECS2
// Package: S1
// Module:  B1SECS2
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1SECS2.h"
#include "B1SECS2MessageS1F17Writable.h"

using namespace BnD;

B1SECS2MessageS1F17Writable::B1SECS2MessageS1F17Writable()
    : B1SECS2MessageWritable(true, 1, 17, "sendMessageS1F17")
{
}

B1SECS2MessageS1F17Writable::~B1SECS2MessageS1F17Writable()
{
}

B1String B1SECS2MessageS1F17Writable::fullName()
{
    return "Request ON-LINE (RONL)";
}

bool B1SECS2MessageS1F17Writable::implWriteData(std::list<std::shared_ptr<B1SECS2Data> >* data)
{
    /* Request ON-LINE (RONL)
    */
    return true;
}
