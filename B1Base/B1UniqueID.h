//
// B1UniqueID.h
//
// Library: B1Base
// Package: B1Base
// Module:  B1Base
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1BASE_UNIQUE_ID_H
#define _B1BASE_UNIQUE_ID_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <B1Base/B1Lock.h>
#include <B1Base/B1String.h>

#if !defined(_WIN32)
#include <pthread.h>
#endif

namespace BnD {
    class B1UniqueID {
    public:
        B1UniqueID();
        virtual ~B1UniqueID();
    private:
        FILE* _fp;
        B1Lock _lock;
        int64 _lastID;
        std::pair<int64, int64> _range; //  pair<begin, end> begin <= x < end.
    private:
        bool writeLastIDToFile();
    public:
        bool initialize(int64 begin, int64 end, const B1String& dirPath, const B1String& fileName);
        void finalize();
        int64 generateNextID();
    };
}   //  !BnD

#endif  // !_B1BASE_UNIQUE_ID_H
