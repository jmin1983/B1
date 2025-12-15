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

void B1GEMVariableSetter::setData(const B1SECS2DataVID& vID)
{
    _vID = std::make_shared<const B1SECS2DataVID>(vID);
}

std::shared_ptr<B1SECS2DataV> B1GEMVariableLIST::implGetValue(const B1GEMDataValueCondition& gemCondition) const
{
    std::shared_ptr<B1SECS2DataV> v(new B1SECS2DataV(SPB1SECS2Data(new B1SECS2DataLIST(label().copy()))));
    if (setValue(gemCondition, static_cast<B1SECS2DataLIST*>(v->get()), &v->_list) != true) {
        return std::shared_ptr<B1SECS2DataV>();
    }
    return v;
}

void B1GEMVariableLIST::addValueList(SPB1SECS2Data data, std::list<SPB1SECS2Data>* valueList) const
{
    valueList->push_back(data);
}

void B1GEMVariableLIST::addValueList(std::shared_ptr<B1SECS2DataV> v, std::list<SPB1SECS2Data>* valueList) const
{
    valueList->push_back(v->data());
    if (v->data()->isFormatList()) {
        valueList->insert(valueList->end(), v->_list.begin(), v->_list.end());
    }
}
