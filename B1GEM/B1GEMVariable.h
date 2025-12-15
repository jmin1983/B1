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

#include <B1SECS2/B1SECS2Data.h>
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

    class B1GEMVariableSetter : public B1GEMVariable {
    protected:
        B1GEMVariableSetter(B1String&& label) : B1GEMVariable(std::move(label)) {}
    public:
        void setData(const B1SECS2DataVID& vID);
    };

    template <typename T, typename U>
    class B1GEMVariableSetterBase : public B1GEMVariableSetter {
    protected:
        B1GEMVariableSetterBase(B1String&& label) : B1GEMVariableSetter(std::move(label)) {}
    private:
        T* createData(const U& data) const { auto t = new T(data); t->setLabel(_label.copy()); return t; }
    protected:
        std::shared_ptr<B1SECS2DataV> implGetValue(const B1GEMDataValueCondition& gemCondition) const final
        {
            U value;
            if (setValue(gemCondition, &value) != true) {
                return std::shared_ptr<B1SECS2DataV>();
            }
            return std::shared_ptr<B1SECS2DataV>(new B1SECS2DataV(SPB1SECS2Data(createData(value))));
        }
    protected:
        virtual bool setValue(const B1GEMDataValueCondition& gemCondition, U* value) const = 0;
    };

    class B1GEMVariableLIST : public B1GEMVariableSetter {
    public:
        B1GEMVariableLIST(B1String&& label) : B1GEMVariableSetter(std::move(label)) {}
    protected:
        std::shared_ptr<B1SECS2DataV> implGetValue(const B1GEMDataValueCondition& gemCondition) const final;
    protected:
        virtual bool setValue(const B1GEMDataValueCondition& gemCondition, B1SECS2DataLIST* value, std::list<SPB1SECS2Data>* valueList) const = 0;
    protected:
        void addValueList(SPB1SECS2Data data, std::list<SPB1SECS2Data>* valueList) const;
        void addValueList(std::shared_ptr<B1SECS2DataV> v, std::list<SPB1SECS2Data>* valueList) const;
    };

    class B1GEMVariableASCII : public B1GEMVariableSetterBase<B1SECS2DataASCII, B1String> {
    public:
        B1GEMVariableASCII(B1String&& label) : B1GEMVariableSetterBase<B1SECS2DataASCII, B1String>(std::move(label)) {}
    };
    class B1GEMVariableUINT8 : public B1GEMVariableSetterBase<B1SECS2DataUINT8, uint8> {
    public:
        B1GEMVariableUINT8(B1String&& label) : B1GEMVariableSetterBase<B1SECS2DataUINT8, uint8>(std::move(label)) {}
    };
    class B1GEMVariableUINT16 : public B1GEMVariableSetterBase<B1SECS2DataUINT16, uint16> {
    public:
        B1GEMVariableUINT16(B1String&& label) : B1GEMVariableSetterBase<B1SECS2DataUINT16, uint16>(std::move(label)) {}
    };

    typedef std::shared_ptr<B1GEMVariable> SPGEMVariable;
}   //  !BnD

#endif  // !_B1GEM_VARIABLE_H
