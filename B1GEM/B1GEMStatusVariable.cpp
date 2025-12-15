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

void B1GEMStatusVariableSetter::setData(const B1SECS2DataSVID& svID, const B1SECS2DataSVNAME& svNAME, const B1SECS2DataUNITS& units)
{
    _svID = std::make_shared<const B1SECS2DataSVID>(svID);
    _svName = std::make_shared<const B1SECS2DataSVNAME>(svNAME);
    _units = std::make_shared<const B1SECS2DataUNITS>(units);
}

B1SECS2DataSV B1GEMStatusVariableLIST::implGetValue() const
{
    B1SECS2DataSV sv(SPB1SECS2Data(new B1SECS2DataLIST(label().copy())));
    setValue(static_cast<B1SECS2DataLIST*>(sv.get()), &sv._list);
    return sv;
}

void B1GEMStatusVariableLIST::addValueList(SPB1SECS2Data data, std::list<SPB1SECS2Data>* valueList) const
{
    valueList->push_back(data);
}

void B1GEMStatusVariableLIST::addValueList(B1SECS2DataSV sv, std::list<SPB1SECS2Data>* valueList) const
{
    valueList->push_back(sv.data());
    if (sv.data()->isFormatList()) {
        valueList->insert(valueList->end(), sv._list.begin(), sv._list.end());
    }
}

void B1GEMStatusVariableLIST::addValueList(std::shared_ptr<B1SECS2DataV> v, std::list<SPB1SECS2Data>* valueList) const
{
    valueList->push_back(v->data());
    if (v->data()->isFormatList()) {
        valueList->insert(valueList->end(), v->_list.begin(), v->_list.end());
    }
}
