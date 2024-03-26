//
// B1GEMAlarmRepository.h
//
// Library: B1GEM
// Package: B1GEM
// Module:  B1GEM
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1GEM_ALARM_REPOSITORY_H
#define _B1GEM_ALARM_REPOSITORY_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <B1GEM/B1GEMAlarm.h>

#include <map>

namespace BnD {
    class B1GEMAlarmRepository {
    public:
        B1GEMAlarmRepository();
        virtual ~B1GEMAlarmRepository();
    protected:
        std::map<B1SECS2DataALID, std::pair<SPB1GEMAlarm, SPB1GEMAlarm> > _alarms;    //  map<ALID, pair<set, cleared> >
    public:
        bool initialize(const std::map<B1SECS2DataALID, std::pair<B1GEMAlarm, B1GEMAlarm> >& alarms); //  map<ALID, pair<set, cleared> >
        void finalize();
        SPB1GEMAlarm findAlarm(const B1SECS2DataALID& alID, bool isSetAlarm) const;
    };
}   //  !BnD

#endif  // !_B1GEM_ALARM_REPOSITORY_H
