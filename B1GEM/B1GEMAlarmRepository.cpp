//
// B1GEMAlarmRepository.cpp
//
// Library: B1GEM
// Package: B1GEM
// Module:  B1GEM
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1GEM.h"
#include "B1GEMAlarmRepository.h"

using namespace BnD;

B1GEMAlarmRepository::B1GEMAlarmRepository()
    : _alarms()
{
}

B1GEMAlarmRepository::~B1GEMAlarmRepository()
{
}

bool B1GEMAlarmRepository::initialize(const std::map<B1SECS2DataALID, std::pair<B1GEMAlarm, B1GEMAlarm> >& alarms)
{
    _alarms.clear();
    for (const auto& alarmPair : alarms) {
        _alarms.insert(std::make_pair(alarmPair.first, std::make_pair(std::make_shared<B1GEMAlarm>(alarmPair.second.first),
                                                                      std::make_shared<B1GEMAlarm>(alarmPair.second.second))));
    }
    return true;
}

void B1GEMAlarmRepository::finalize()
{
    _alarms.clear();
}

SPB1GEMAlarm B1GEMAlarmRepository::findAlarm(const B1SECS2DataALID& alID, bool isSetAlarm) const
{
    auto itr = _alarms.find(alID);
    return itr != _alarms.end() ? isSetAlarm ? itr->second.first : itr->second.second : SPB1GEMAlarm();
}
