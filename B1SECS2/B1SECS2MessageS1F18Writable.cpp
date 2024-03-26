//
// B1SECS2MessageS1F18Writable.cpp
//
// Library: B1SECS2
// Package: S1
// Module:  B1SECS2
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1SECS2.h"
#include "B1SECS2MessageS1F18Writable.h"

using namespace BnD;

B1SECS2MessageS1F18Writable::B1SECS2MessageS1F18Writable(const B1SECS2DataONLACK& onlAck)
    : B1SECS2MessageWritable(false, 1, 18, "sendMessageS1F18")
    , _onlAck(onlAck)
{
}

B1SECS2MessageS1F18Writable::~B1SECS2MessageS1F18Writable()
{
}

B1String B1SECS2MessageS1F18Writable::fullName()
{
    return "ON-LINE Acknowledge (ONLA)";
}

bool B1SECS2MessageS1F18Writable::implWriteData(std::list<std::shared_ptr<B1SECS2Data> >* data)
{
    /* ON-LINE Acknowledge (ONLA)
    <ONLACK>
    */
    addData(_onlAck, data);
    return true;
}
