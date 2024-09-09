//
// B1DataType.h
//
// Library: B1Base
// Package: B1Base
// Module:  B1Base
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1BASE_DATA_TYPE_H
#define _B1BASE_DATA_TYPE_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <B1Base/B1Archive.h>

#include <vector>

namespace BnD {
    typedef std::pair<const B1String, bool> DataBool;
    typedef std::pair<const B1String, int8> DataInt8;
    typedef std::pair<const B1String, int16> DataInt16;
    typedef std::pair<const B1String, int32> DataInt32;
    typedef std::pair<const B1String, int64> DataInt64;
    typedef std::pair<const B1String, uint8> DataUint8;
    typedef std::pair<const B1String, uint16> DataUint16;
    typedef std::pair<const B1String, uint32> DataUint32;
    typedef std::pair<const B1String, uint64> DataUint64;
    typedef std::pair<const B1String, float32> DataFloat32;
    typedef std::pair<const B1String, float64> DataFloat64;
    typedef std::pair<const B1String, B1String> DataString;
    typedef std::pair<const B1String, std::vector<int32> > DataVectorInt32;
    typedef std::pair<const B1String, std::vector<B1String> > DataVectorString;
}   //  !BnD

#endif  // !_B1BASE_DATA_TYPE_H
