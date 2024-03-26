//
// B1GEMAlarm.cpp
//
// Library: B1GEM
// Package: B1GEM
// Module:  B1GEM
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1GEM.h"
#include "B1GEMAlarm.h"

using namespace BnD;

B1GEMAlarm::B1GEMAlarm(const B1GEMAlarm& alarm, bool deepCopy)
    : _alID(static_cast<const B1SECS2DataALID*>(alarm._alID->clone()))
    , _alCD(static_cast<const B1SECS2DataALCD*>(alarm._alCD->clone()))
    , _alTX(static_cast<const B1SECS2DataALTX*>(alarm._alTX->clone()))
    , _ceID(static_cast<const B1SECS2DataCEID*>(alarm._ceID->clone()))
{
}

B1GEMAlarm::B1GEMAlarm(const B1SECS2DataALID& alID, const B1SECS2DataALCD& alCD, const B1SECS2DataALTX& alTX, const B1SECS2DataCEID& ceID)
    : _alID(std::make_shared<const B1SECS2DataALID>(alID))
    , _alCD(std::make_shared<const B1SECS2DataALCD>(alCD))
    , _alTX(std::make_shared<const B1SECS2DataALTX>(alTX))
    , _ceID(std::make_shared<const B1SECS2DataCEID>(ceID))
{
}

B1GEMAlarm::~B1GEMAlarm()
{
}
