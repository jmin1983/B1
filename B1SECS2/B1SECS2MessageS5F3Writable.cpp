//
// B1SECS2MessageS5F3Writable.cpp
//
// Library: B1SECS2
// Package: S5
// Module:  B1SECS2
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1SECS2.h"
#include "B1SECS2MessageS5F3Writable.h"

using namespace BnD;

B1SECS2MessageS5F3Writable::B1SECS2MessageS5F3Writable(const B1SECS2DataALED& alED, const B1SECS2DataALID& alID)
    : B1SECS2MessageWritable(true, 5, 3, "sendMessageS5F3")
    , _alED(alED)
    , _alID(alID)
{
}

B1SECS2MessageS5F3Writable::~B1SECS2MessageS5F3Writable()
{
}

B1String B1SECS2MessageS5F3Writable::fullName()
{
    return "Enable/Disable Alarm Send (EAS)";
}

bool B1SECS2MessageS5F3Writable::implWriteData(std::list<std::shared_ptr<B1SECS2Data> >* data)
{
    /* Enable/Disable Alarm Send (EAS)
    <L 2
        <ALED>
        <ALID>
    >
    */
    addList(2, data);
    addData(_alED, data);
    addData(_alID, data);
    return true;
}
