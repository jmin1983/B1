//
// B1Object.h
//
// Library: B1Base
// Package: B1Base
// Module:  B1Base
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1BASE_OBJECT_H
#define _B1BASE_OBJECT_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <B1Base/B1Archive.h>
#include <B1Base/B1DataType.h>

#include <vector>

namespace BnD {
    class B1String;
    class B1Object {
    public:
        B1Object() {}
        virtual ~B1Object() {}
    protected:
        template<typename T>
        void writeDataToArchive(const T& data, B1Archive* archive) const
        {
            archive->writeData(data.first, data.second);
        }
        template<typename T>
        void writeDataToArchive(const B1String& key, const T& value, B1Archive* archive) const
        {
            archive->writeData(key, value);
        }
        template<typename T>
        bool readDataFromArchive(const B1Archive& archive, T* data)
        {
            return archive.readData(data->first, &data->second);
        }
        template<typename T>
        bool readDataFromArchive(const B1String& key, const B1Archive& archive, T* value)
        {
            return archive.readData(key, value);
        }
        template <typename T>
        bool readDataFromArchiveCastInt32(const B1String& key, const B1Archive& archive, T* value)
        {
            int32 temp = 0;
            bool result = readDataFromArchive(key, archive, &temp);
            *value = static_cast<T>(temp);
            return result;
        }
        template<typename T>
        void writeObjectArrayToArchive(const B1String& key, const std::vector<T>& value, B1Archive* archive) const
        {
            archive->writeObjectArray(key, value);
        }
        template<typename T>
        void writeObjectArrayToArchive(const T& data, B1Archive* archive) const
        {
            archive->writeObjectArray(data.first, data.second);
        }
        template<typename T>
        void readObjectArrayFromArchive(const B1String& key, const B1Archive& archive, std::vector<T>* value)
        {
            archive.readObjectArray(key, value);
        }
        template<typename T>
        void readObjectArrayFromArchive(const B1Archive& archive, T* data)
        {
            archive.readObjectArray(data->first, &data->second);
        }
    public:
        virtual void archiveTo(B1Archive* archive) const {}
        virtual void unarchiveFrom(const B1Archive& archive) {}
    public:
        bool archiveToString(B1String* result, bool pretty = false) const;
        void unarchiveFromString(const B1String& string);
    };
}   //  !BnD

#endif  // !_B1BASE_OBJECT_H
