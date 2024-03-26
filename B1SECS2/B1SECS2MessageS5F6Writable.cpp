//
// B1SECS2MessageS5F6Writable.cpp
//
// Library: B1SECS2
// Package: S5
// Module:  B1SECS2
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1SECS2.h"
#include "B1SECS2MessageS5F6Writable.h"

using namespace BnD;

B1SECS2MessageS5F6Writable::B1SECS2MessageS5F6Writable(const std::map<B1SECS2DataALID, std::pair<B1SECS2DataALCD, B1SECS2DataALTX> >& alarms)
    : B1SECS2MessageWritable(false, 5, 6, "sendMessageS5F6")
    , _alarms(alarms)
{
}

B1SECS2MessageS5F6Writable::~B1SECS2MessageS5F6Writable()
{
}

B1String B1SECS2MessageS5F6Writable::fullName()
{
    return "List Alarm Data (LAD)";
}

bool B1SECS2MessageS5F6Writable::implWriteData(std::list<std::shared_ptr<B1SECS2Data> >* data)
{
    /* List Alarm Data (LAD)
    <L n
        <L 3
            <ALCD>
            <ALID>
            <ALTX>
        >
    >
    */
    addList(_alarms.size(), data);
    for (const auto& alarmsPair : _alarms) {
        addList(3, data);
        addData(alarmsPair.second.first, data);
        addData(alarmsPair.first, data);
        addData(alarmsPair.second.second, data);
    }
    return true;
}
