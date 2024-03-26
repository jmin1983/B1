//
// B1SECS2MessageS5F5Writable.cpp
//
// Library: B1SECS2
// Package: S5
// Module:  B1SECS2
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1SECS2.h"
#include "B1SECS2MessageS5F5Writable.h"

using namespace BnD;

B1SECS2MessageS5F5Writable::B1SECS2MessageS5F5Writable(const B1SECS2DataALID& alID)
    : B1SECS2MessageWritable(true, 5, 5, "sendMessageS5F5")
    , _alID(alID)
{
}

B1SECS2MessageS5F5Writable::~B1SECS2MessageS5F5Writable()
{
}

B1String B1SECS2MessageS5F5Writable::fullName()
{
    return "List Alarm Request (LAR)";
}

bool B1SECS2MessageS5F5Writable::implWriteData(std::list<std::shared_ptr<B1SECS2Data> >* data)
{
    /* List Alarm Request (LAR)
    <ALID>
    */
    addData(_alID, data);
    return true;
}
