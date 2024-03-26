//
// B1GEMStatusVariable.h
//
// Library: B1GEM
// Package: B1GEM
// Module:  B1GEM
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1GEM_STATUS_VARIABLE_H
#define _B1GEM_STATUS_VARIABLE_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <B1SECS2/B1SECS2DataTypes.h>

namespace BnD {
    class B1GEMStatusVariable {
    protected:
        B1GEMStatusVariable(B1String&& label);
    public:
        virtual ~B1GEMStatusVariable();
    protected:
        const B1String _label;
        std::shared_ptr<const B1SECS2DataSVID> _svID;
        std::shared_ptr<const B1SECS2DataSVNAME> _svName;
        std::shared_ptr<const B1SECS2DataUNITS> _units;
    protected:
        virtual B1SECS2DataSV implGetValue() const = 0;
    public:
        bool operator<(const B1GEMStatusVariable& v) const { return *_svID < *v._svID; }
    public:
        const B1String& label() const { return _label; }
        const B1SECS2DataSVID& svID() const { return *_svID; }
        const B1SECS2DataSVNAME& svName() const { return *_svName; }
        const B1SECS2DataUNITS& units() const { return *_units; }
        B1SECS2DataSV getValue() const;
    };

    typedef std::shared_ptr<B1GEMStatusVariable> SPB1GEMStatusVariable;
}   //  !BnD

#endif  // !_B1GEM_STATUS_VARIABLE_H
