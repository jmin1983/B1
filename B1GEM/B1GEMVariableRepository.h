//
// B1GEMVariableRepository.h
//
// Library: B1GEM
// Package: B1GEM
// Module:  B1GEM
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1GEM_VARIABLE_REPOSITORY_H
#define _B1GEM_VARIABLE_REPOSITORY_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <B1GEM/B1GEMEquipmentConstant.h>
#include <B1GEM/B1GEMStatusVariable.h>
#include <B1GEM/B1GEMVariable.h>

#include <map>

namespace BnD {
    class B1GEMVariableRepository {
    public:
        B1GEMVariableRepository();
        virtual ~B1GEMVariableRepository();
    protected:
        std::map<const B1SECS2DataECID, SPGEMEquipmentConstant> _ecs;
        std::map<const B1SECS2DataSVID, SPB1GEMStatusVariable> _svs;
        std::map<const B1SECS2DataVID, SPGEMVariable> _vs;
    public:
        bool initialize(const std::map<const B1SECS2DataECID, SPGEMEquipmentConstant>& ecs,
                        const std::map<const B1SECS2DataSVID, SPB1GEMStatusVariable>& svs,
                        const std::map<const B1SECS2DataVID, SPGEMVariable>& vs);
        void finalize();
        SPGEMEquipmentConstant findEquipmentConstant(const B1SECS2DataECID& ecID) const;
        SPB1GEMStatusVariable findStatusVariable(const B1SECS2DataSVID& svID) const;
        std::map<B1SECS2DataSVID, std::pair<B1SECS2DataSVNAME, B1SECS2DataUNITS> > findStatusVariables(const std::vector<B1SECS2DataSVID>& svIDs) const;
        SPGEMVariable findVariable(const B1SECS2DataVID& vID) const;
    };
}   //  !BnD

#endif  // !_B1GEM_VARIABLE_REPOSITORY_H
