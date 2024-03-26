//
// B1GEMAlarm.h
//
// Library: B1GEM
// Package: B1GEM
// Module:  B1GEM
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1GEM_ALARM_H
#define _B1GEM_ALARM_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <B1SECS2/B1SECS2DataTypes.h>

namespace BnD {
    class B1GEMAlarm {
    protected:
        B1GEMAlarm(const B1GEMAlarm& alarm, bool deepCopy);
    public:
        B1GEMAlarm(const B1SECS2DataALID& alID, const B1SECS2DataALCD& alCD, const B1SECS2DataALTX& alTX, const B1SECS2DataCEID& ceID);
        virtual ~B1GEMAlarm();
    protected:
        std::shared_ptr<const B1SECS2DataALID> _alID;
        std::shared_ptr<const B1SECS2DataALCD> _alCD;
        std::shared_ptr<const B1SECS2DataALTX> _alTX;
        std::shared_ptr<const B1SECS2DataCEID> _ceID;
    public:
        bool operator<(const B1GEMAlarm& a) const { return *_alID < *a._alID; }
        virtual B1GEMAlarm* clone() { return new B1GEMAlarm(*this, true); }
    public:
        const B1SECS2DataALID& alID() const { return *_alID; }
        const B1SECS2DataALCD& alCD() const { return *_alCD; }
        const B1SECS2DataALTX& alTX() const { return *_alTX; }
        const B1SECS2DataCEID& ceID() const { return *_ceID; }
    };

    typedef std::shared_ptr<B1GEMAlarm> SPB1GEMAlarm;
}   //  !BnD

#endif  // !_B1GEM_ALARM_H
