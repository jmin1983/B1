//
// B1GEMVariable.h
//
// Library: B1GEM
// Package: B1GEM
// Module:  B1GEM
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1GEM_VARIABLE_H
#define _B1GEM_VARIABLE_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <B1SECS2/B1SECS2DataTypes.h>

namespace BnD {
    struct B1GEMDataValueCondition;
    class B1GEMVariable {
    protected:
        B1GEMVariable(B1String&& label);
    public:
        virtual ~B1GEMVariable();
    protected:
        const B1String _label;
        std::shared_ptr<const B1SECS2DataVID> _vID;
    protected:
        virtual bool isValidGEMCondition(const B1GEMDataValueCondition& gemCondition) const { return true; }
        virtual std::shared_ptr<B1SECS2DataV> implGetValue(const B1GEMDataValueCondition& gemCondition) const = 0;
    public:
        bool operator<(const B1GEMVariable& v) const { return *_vID < *v._vID; }
    public:
        const B1String& label() const { return _label; }
        const B1SECS2DataVID& vID() const { return *_vID; }
        std::shared_ptr<B1SECS2DataV> getValue(const B1GEMDataValueCondition& gemCondition) const;    //  return NULL if no value
    };

    typedef std::shared_ptr<B1GEMVariable> SPGEMVariable;
}   //  !BnD

#endif  // !_B1GEM_VARIABLE_H
