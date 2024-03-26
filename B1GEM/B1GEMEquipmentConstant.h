//
// B1GEMEquipmentConstant.h
//
// Library: B1GEM
// Package: B1GEM
// Module:  B1GEM
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1GEM_EQUIPMENT_CONSTANT_H
#define _B1GEM_EQUIPMENT_CONSTANT_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <B1SECS2/B1SECS2Data.h>
#include <B1SECS2/B1SECS2DataTypes.h>

namespace BnD {
    class B1GEMEquipmentConstant {
    protected:
        B1GEMEquipmentConstant(B1String&& label);
    public:
        virtual ~B1GEMEquipmentConstant();
    protected:
        const B1String _label;
        std::shared_ptr<const B1SECS2DataECID> _ecID;
        std::shared_ptr<const B1SECS2DataECNAME> _ecName;
        std::shared_ptr<const B1SECS2DataECMIN> _ecMin;
        std::shared_ptr<const B1SECS2DataECMAX> _ecMax;
        std::shared_ptr<const B1SECS2DataECDEF> _ecDef;
        std::shared_ptr<const B1SECS2DataUNITS> _units;
    protected:
        virtual SPB1SECS2Data implGetValue() const = 0;
        virtual bool implModifyValue(SPB1SECS2Data value) = 0;
    public:
        bool operator<(const B1GEMEquipmentConstant& v) const { return *_ecID < *v._ecID; }
    public:
        const B1String& label() const { return _label; }
        const B1SECS2DataECID& ecID() const { return *_ecID; }
        const B1SECS2DataECNAME& ecName() const { return *_ecName; }
        const B1SECS2DataECMIN& ecMin() const { return *_ecMin; }
        const B1SECS2DataECMAX& ecMax() const { return *_ecMax; }
        const B1SECS2DataECDEF& ecDef() const { return *_ecDef; }
        const B1SECS2DataUNITS& units() const { return *_units; }
        SPB1SECS2Data getValue() const;
        bool modifyValue(SPB1SECS2Data value);
    };

    class B1GEMEquipmentConstantSetter : public B1GEMEquipmentConstant {
    protected:
        B1GEMEquipmentConstantSetter(B1String&& label) : B1GEMEquipmentConstant(std::move(label)) {}
    public:
        virtual ~B1GEMEquipmentConstantSetter() {}
    protected:
        virtual B1SECS2Data* createMinValue() const = 0;
        virtual B1SECS2Data* createMaxValue() const = 0;
        virtual B1SECS2Data* createDefValue() const = 0;
    public:
        void setData(const B1SECS2DataECID& ecID, const B1SECS2DataECNAME& ecNAME, const B1SECS2DataUNITS& units);
    };

    template <typename T, typename U>
    class B1GEMEquipmentConstantSetterBase : public B1GEMEquipmentConstantSetter {
    protected:
        B1GEMEquipmentConstantSetterBase(B1String&& label) : B1GEMEquipmentConstantSetter(std::move(label)) {}
    public:
        virtual ~B1GEMEquipmentConstantSetterBase() {}
    private:
        T* createData(const U& data) const { auto t = new T(data); t->setLabel(_label.copy()); return t; }
    protected:
        B1SECS2Data* createMinValue() const final { return createData(minValue()); }
        B1SECS2Data* createMaxValue() const final { return createData(maxValue()); }
        B1SECS2Data* createDefValue() const final { return createData(defValue()); }
        SPB1SECS2Data implGetValue() const final { return SPB1SECS2Data(createData(value())); }
        bool implModifyValue(SPB1SECS2Data value) final
        {
            bool result = false;
            switch (value->format()) {
                case B1SECS2Consts::DATA_FORMAT_ASCII:
                    result = setValue(static_cast<const B1SECS2DataASCII*>(value.get())->data());
                    break;
                case B1SECS2Consts::DATA_FORMAT_UINT8:
                    result = setValue(static_cast<const B1SECS2DataUINT8*>(value.get())->data());
                    break;
                default:
                    assert(false);
                    break;
            }
            return result;
        }
    protected:
        virtual U minValue() const = 0;
        virtual U maxValue() const = 0;
        virtual U defValue() const = 0;
        virtual U value() const = 0;
        virtual bool setValue(const B1String& value) { assert(false); return false; }
        virtual bool setValue(uint8 value) { assert(false); return false; }
    };

    class B1GEMEquipmentConstantASCII : public B1GEMEquipmentConstantSetterBase<B1SECS2DataASCII, B1String> {
    protected:
        B1GEMEquipmentConstantASCII(B1String&& label) : B1GEMEquipmentConstantSetterBase<B1SECS2DataASCII, B1String>(std::move(label)) {}
    protected:
        virtual B1String minValue() const override { return ""; }
        virtual B1String maxValue() const override { return ""; }
        virtual B1String defValue() const override { return ""; }
        virtual bool setValue(const B1String& value) override = 0;
    };
    class B1GEMEquipmentConstantUINT8 : public B1GEMEquipmentConstantSetterBase<B1SECS2DataUINT8, uint8> {
    protected:
        B1GEMEquipmentConstantUINT8(B1String&& label) : B1GEMEquipmentConstantSetterBase<B1SECS2DataUINT8, uint8>(std::move(label)) {}
    protected:
        virtual bool setValue(uint8 value) override = 0;
    };

    typedef std::shared_ptr<B1GEMEquipmentConstant> SPGEMEquipmentConstant;
}   //  !BnD

#endif  // !_B1GEM_EQUIPMENT_CONSTANT_H
