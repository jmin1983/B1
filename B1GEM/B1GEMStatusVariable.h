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

#include <B1SECS2/B1SECS2Data.h>
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

    class B1GEMStatusVariableSetter : public B1GEMStatusVariable {
    protected:
        B1GEMStatusVariableSetter(B1String&& label) : B1GEMStatusVariable(std::move(label)) {}
    public:
        void setData(const B1SECS2DataSVID& svID, const B1SECS2DataSVNAME& svNAME, const B1SECS2DataUNITS& units);
    };

    template <typename T, typename U>
    class B1GEMStatusVariableSetterBase : public B1GEMStatusVariableSetter {
    protected:
        B1GEMStatusVariableSetterBase(B1String&& label) : B1GEMStatusVariableSetter(std::move(label)) {}
    private:
        T* createData(const U& data) const { auto t = new T(data); t->setLabel(_label.copy()); return t; }
    protected:
        B1SECS2DataSV implGetValue() const final { return B1SECS2DataSV(SPB1SECS2Data(createData(value()))); }
    protected:
        virtual U value() const = 0;
    };

    class B1GEMStatusVariableLIST : public B1GEMStatusVariableSetter {
    public:
        B1GEMStatusVariableLIST(B1String&& label) : B1GEMStatusVariableSetter(std::move(label)) {}
    protected:
        B1SECS2DataSV implGetValue() const final;
    protected:
        virtual void setValue(B1SECS2DataLIST* value, std::list<SPB1SECS2Data>* valueList) const = 0;
    protected:
        void addValueList(SPB1SECS2Data data, std::list<SPB1SECS2Data>* valueList) const;
        void addValueList(B1SECS2DataSV sv, std::list<SPB1SECS2Data>* valueList) const;
        void addValueList(std::shared_ptr<B1SECS2DataV> v, std::list<SPB1SECS2Data>* valueList) const;
    };

    class B1GEMStatusVariableASCII : public B1GEMStatusVariableSetterBase<B1SECS2DataASCII, B1String> {
    public:
        B1GEMStatusVariableASCII(B1String&& label) : B1GEMStatusVariableSetterBase<B1SECS2DataASCII, B1String>(std::move(label)) {}
    };
    class B1GEMStatusVariableUINT8 : public B1GEMStatusVariableSetterBase<B1SECS2DataUINT8, uint8> {
    public:
        B1GEMStatusVariableUINT8(B1String&& label) : B1GEMStatusVariableSetterBase<B1SECS2DataUINT8, uint8>(std::move(label)) {}
    };
    class B1GEMStatusVariableUINT16 : public B1GEMStatusVariableSetterBase<B1SECS2DataUINT16, uint16> {
    public:
        B1GEMStatusVariableUINT16(B1String&& label) : B1GEMStatusVariableSetterBase<B1SECS2DataUINT16, uint16>(std::move(label)) {}
    };
    class B1GEMStatusVariableBIN : public B1GEMStatusVariableSetterBase<B1SECS2DataBIN, uint8> {
    public:
        B1GEMStatusVariableBIN(B1String&& label) : B1GEMStatusVariableSetterBase<B1SECS2DataBIN, uint8>(std::move(label)) {}
    };

    typedef std::shared_ptr<B1GEMStatusVariable> SPB1GEMStatusVariable;
}   //  !BnD

#endif  // !_B1GEM_STATUS_VARIABLE_H
