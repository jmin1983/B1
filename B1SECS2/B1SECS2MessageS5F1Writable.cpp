//
// B1SECS2MessageS5F1Writable.cpp
//
// Library: B1SECS2
// Package: S5
// Module:  B1SECS2
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1SECS2.h"
#include "B1SECS2MessageS5F1Writable.h"

using namespace BnD;

B1SECS2MessageS5F1Writable::B1SECS2MessageS5F1Writable(const B1SECS2DataALCD& alCD, const B1SECS2DataALID& alID, const B1SECS2DataALTX& alTX)
    : B1SECS2MessageWritable(true, 5, 1, "sendMessageS5F1")
    , _alCD(alCD)
    , _alID(alID)
    , _alTX(alTX)
{
}

B1SECS2MessageS5F1Writable::~B1SECS2MessageS5F1Writable()
{
}

B1String B1SECS2MessageS5F1Writable::fullName()
{
    return "Alarm Report Send (ARS)";
}

bool B1SECS2MessageS5F1Writable::implWriteData(std::list<std::shared_ptr<B1SECS2Data> >* data)
{
    /* Alarm Report Send (ARS)
    W
    <L 3
        <ALCD>
        <ALID>
        <ALTX>
    >
    */
    addList(3, data);
    addData(_alCD, data);
    addData(_alID, data);
    addData(_alTX, data);
    return true;
}
