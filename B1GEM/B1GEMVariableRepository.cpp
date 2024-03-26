//
// B1GEMVariableRepository.cpp
//
// Library: B1GEM
// Package: B1GEM
// Module:  B1GEM
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1GEM.h"
#include "B1GEMVariableRepository.h"

using namespace BnD;

B1GEMVariableRepository::B1GEMVariableRepository()
    : _ecs()
    , _svs()
    , _vs()
{
}

B1GEMVariableRepository::~B1GEMVariableRepository()
{
}

bool B1GEMVariableRepository::initialize(const std::map<const B1SECS2DataECID, SPGEMEquipmentConstant>& ecs,
                                       const std::map<const B1SECS2DataSVID, SPB1GEMStatusVariable>& svs,
                                       const std::map<const B1SECS2DataVID, SPGEMVariable>& vs)
{
    if (_ecs.empty() != true || _svs.empty() != true || _vs.empty() != true) {
        return false;
    }
    _ecs.insert(ecs.begin(), ecs.end());
    _svs.insert(svs.begin(), svs.end());
    _vs.insert(vs.begin(), vs.end());
    return true;
}

void B1GEMVariableRepository::finalize()
{
    _vs.clear();
    _svs.clear();
    _ecs.clear();
}

SPGEMEquipmentConstant B1GEMVariableRepository::findEquipmentConstant(const B1SECS2DataECID& ecID) const
{
    auto itr = _ecs.find(ecID);
    return itr != _ecs.end() ? itr->second : SPGEMEquipmentConstant();
}

SPB1GEMStatusVariable B1GEMVariableRepository::findStatusVariable(const B1SECS2DataSVID& svID) const
{
    auto itr = _svs.find(svID);
    return itr != _svs.end() ? itr->second : SPB1GEMStatusVariable();
}

std::map<B1SECS2DataSVID, std::pair<B1SECS2DataSVNAME, B1SECS2DataUNITS> > B1GEMVariableRepository::findStatusVariables(const std::vector<B1SECS2DataSVID>& svIDs) const
{
    std::map<B1SECS2DataSVID, std::pair<B1SECS2DataSVNAME, B1SECS2DataUNITS> > result;
    for (const auto& svID : svIDs) {
        auto itr = _svs.find(svID);
        if (itr != _svs.end()) {
            result.insert(std::make_pair(svID, std::make_pair(itr->second->svName(), itr->second->units())));
        }
    }
    return result;
}

SPGEMVariable B1GEMVariableRepository::findVariable(const B1SECS2DataVID& vID) const
{
    auto itr = _vs.find(vID);
    return itr != _vs.end() ? itr->second : SPGEMVariable();
}
