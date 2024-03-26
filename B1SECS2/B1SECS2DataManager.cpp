//
// B1SECS2DataManager.cpp
//
// Library: B1SECS2
// Package: B1SECS2
// Module:  B1SECS2
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1SECS2.h"
#include "B1SECS2DataManager.h"
#include "B1SECS2Data.h"

using namespace BnD;

B1SECS2DataManager::B1SECS2DataManager()
    : _originalDataTypes()
{
}

B1SECS2DataManager::~B1SECS2DataManager()
{
}

bool B1SECS2DataManager::isCreateExceptionDataType(B1SECS2DataBase::INDEX index) const
{
    switch (index) {
        case B1SECS2DataBase::INDEX_CEPVAL:
        case B1SECS2DataBase::INDEX_CPVAL:
        case B1SECS2DataBase::INDEX_ECDEF:
        case B1SECS2DataBase::INDEX_ECMAX:
        case B1SECS2DataBase::INDEX_ECMIN:
        case B1SECS2DataBase::INDEX_ECV:
        case B1SECS2DataBase::INDEX_SV:
        case B1SECS2DataBase::INDEX_V:
            return true;
        default:
            break;
    }
    return false;
}

bool B1SECS2DataManager::initialize()
{
    for (int32 i = B1SECS2DataBase::INDEX_SEPERATOR_BEGIN; i < B1SECS2DataBase::INDEX_SEPERATOR_END; ++i) {
        auto index = static_cast<B1SECS2DataBase::INDEX>(i);
        if (isCreateExceptionDataType(index) != true) {
            std::shared_ptr<B1SECS2Data> originalData(createOriginalData(index));
            if (originalData == NULL) {
                assert(false);
                return false;
            }
            assert(originalData->label().isEmpty() != true);
            _originalDataTypes.insert(std::make_pair(index, originalData));
        }
    }
    return true;
}

void B1SECS2DataManager::finalize()
{
    _originalDataTypes.clear();
}

std::shared_ptr<B1SECS2Data> B1SECS2DataManager::getNewData(B1SECS2DataBase::INDEX dataIndex) const
{
    auto itr = _originalDataTypes.find(dataIndex);
    return itr != _originalDataTypes.end() ? std::shared_ptr<B1SECS2Data>(itr->second->clone()) : std::shared_ptr<B1SECS2Data>();
}
