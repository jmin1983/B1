//
// B1GEMVariable.cpp
//
// Library: B1GEM
// Package: B1GEM
// Module:  B1GEM
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1GEM.h"
#include "B1GEMVariable.h"

#include <B1SECS2/B1SECS2Data.h>

using namespace BnD;

B1GEMVariable::B1GEMVariable(B1String&& label)
    : _label(label)
    , _vID()
{
}

B1GEMVariable::~B1GEMVariable()
{
}

std::shared_ptr<B1SECS2DataV> B1GEMVariable::getValue(const B1GEMDataValueCondition& gemCondition) const
{
    if (isValidGEMCondition(gemCondition) != true) {
        assert(false);
        return std::shared_ptr<B1SECS2DataV>();
    }
    return implGetValue(gemCondition);
}
