//
// B1GEMEquipmentConstant.cpp
//
// Library: B1GEM
// Package: B1GEM
// Module:  B1GEM
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1GEM.h"
#include "B1GEMEquipmentConstant.h"

#include <B1SECS2/B1SECS2Data.h>

using namespace BnD;

B1GEMEquipmentConstant::B1GEMEquipmentConstant(B1String&& label)
    : _label(label)
    , _ecID()
    , _ecName()
    , _ecMin()
    , _ecMax()
    , _ecDef()
    , _units()
{
}

B1GEMEquipmentConstant::~B1GEMEquipmentConstant()
{
}

SPB1SECS2Data B1GEMEquipmentConstant::getValue() const
{
    return implGetValue();
}

bool B1GEMEquipmentConstant::modifyValue(SPB1SECS2Data value)
{
    return implModifyValue(value);
}

void B1GEMEquipmentConstantSetter::setData(const B1SECS2DataECID& ecID, const B1SECS2DataECNAME& ecNAME, const B1SECS2DataUNITS& units)
{
    _ecID = std::make_shared<const B1SECS2DataECID>(ecID);
    _ecName = std::make_shared<const B1SECS2DataECNAME>(ecNAME);
    _ecMin = std::make_shared<const B1SECS2DataECMIN>(SPB1SECS2Data(createMinValue()));
    _ecMax = std::make_shared<const B1SECS2DataECMAX>(SPB1SECS2Data(createMaxValue()));
    _ecDef = std::make_shared<const B1SECS2DataECDEF>(SPB1SECS2Data(createDefValue()));
    _units = std::make_shared<const B1SECS2DataUNITS>(units);
}
