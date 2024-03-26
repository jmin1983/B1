//
// B1GEMStatusVariable.cpp
//
// Library: B1GEM
// Package: B1GEM
// Module:  B1GEM
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1GEM.h"
#include "B1GEMStatusVariable.h"

using namespace BnD;

B1GEMStatusVariable::B1GEMStatusVariable(B1String&& label)
    : _label(label)
    , _svID()
    , _svName()
    , _units()
{
}

B1GEMStatusVariable::~B1GEMStatusVariable()
{
}

B1SECS2DataSV B1GEMStatusVariable::getValue() const
{
    return implGetValue();
}
