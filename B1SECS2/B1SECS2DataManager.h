//
// B1SECS2DataManager.h
//
// Library: B1SECS2
// Package: B1SECS2
// Module:  B1SECS2
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1SECS2_DATA_MANAGER_H
#define _B1SECS2_DATA_MANAGER_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <B1SECS2/B1SECS2DataTypes.h>

#include <map>

namespace BnD {
    class B1SECS2Data;
    class B1SECS2DataManager {
    public:
        B1SECS2DataManager();
        virtual ~B1SECS2DataManager();
    protected:
        std::map<B1SECS2DataBase::INDEX, std::shared_ptr<B1SECS2Data> > _originalDataTypes;
    protected:
        virtual B1SECS2Data* createOriginalData(B1SECS2DataBase::INDEX index) const = 0;
    protected:
        bool isCreateExceptionDataType(B1SECS2DataBase::INDEX index) const;
    public:
        bool initialize();
        void finalize();
        std::shared_ptr<B1SECS2Data> getNewData(B1SECS2DataBase::INDEX dataIndex) const;
    };
}   //  !BnD

#endif  // !_B1SECS2_DATA_MANAGER_H
