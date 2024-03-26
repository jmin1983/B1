//
// B1SECS2DataTypes.cpp
//
// Library: B1SECS2
// Package: B1SECS2
// Module:  B1SECS2
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1SECS2.h"
#include "B1SECS2DataTypes.h"
#include "B1SECS2Data.h"
#include "B1SECS2DataManager.h"

using namespace BnD;
using namespace B1SECS2Consts;

B1SECS2DataBase::B1SECS2DataBase(INDEX index, const B1SECS2DataManager* manager) : _index(index), _data(manager->getNewData(index))
{
    assert(manager != NULL);
}

B1SECS2DataBase::B1SECS2DataBase(INDEX index, std::shared_ptr<B1SECS2Data> data) : _index(index), _data(data)
{
}

B1SECS2DataBase::~B1SECS2DataBase()
{
}

bool B1SECS2DataBase::operator<(const B1SECS2DataBase& a) const
{
    if (data()->format() == a->format()) {
        switch (data()->format()) {
            case B1SECS2Consts::DATA_FORMAT_UINT32:
                return static_cast<const B1SECS2DataUINT32&>(*get()) < static_cast<const B1SECS2DataUINT32&>(*a.get());
            case B1SECS2Consts::DATA_FORMAT_UINT16:
                return static_cast<const B1SECS2DataUINT16&>(*get()) < static_cast<const B1SECS2DataUINT16&>(*a.get());
            case B1SECS2Consts::DATA_FORMAT_UINT8:
                return static_cast<const B1SECS2DataUINT8&>(*get()) < static_cast<const B1SECS2DataUINT8&>(*a.get());
            case B1SECS2Consts::DATA_FORMAT_ASCII:
                return static_cast<const B1SECS2DataASCII&>(*get()) < static_cast<const B1SECS2DataASCII&>(*a.get());
            default:
                break;
        }
    }
    assert(false);  //  other type is not allowed.
    return _index < a._index;
}

B1SECS2DataBase* B1SECS2DataBase::clone() const
{
    return new B1SECS2DataBase(_index, std::shared_ptr<B1SECS2Data>(data()->clone()));
}
