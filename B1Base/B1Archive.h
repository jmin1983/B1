//
// B1Archive.h
//
// Library: B1Base
// Package: B1Base
// Module:  B1Base
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1BASE_ARCHIVE_H
#define _B1BASE_ARCHIVE_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <vector>

namespace BnD {
    class B1Object;
    class B1String;
    class B1Archive {
    private:
        template <typename T>
        B1Archive(const T& t);
    public:
        B1Archive();
        virtual ~B1Archive();
    private:
        class B1ArchiveNode* _rootNode;
    public:
#define DECLARE_ARCHIVE_WRITE_VALUE_FUNC(type) void writeData(const B1String &key, const type &value);
#define DECLARE_ARCHIVE_READ_VALUE_FUNC(type) bool readData(const B1String &key, type *value) const;
#define DECLARE_ARCHIVE_VALUE_FUNC(type) DECLARE_ARCHIVE_WRITE_VALUE_FUNC(type) DECLARE_ARCHIVE_READ_VALUE_FUNC(type)
        DECLARE_ARCHIVE_VALUE_FUNC(bool);
        DECLARE_ARCHIVE_VALUE_FUNC(int8);
        DECLARE_ARCHIVE_VALUE_FUNC(int16);
        DECLARE_ARCHIVE_VALUE_FUNC(int32);
        DECLARE_ARCHIVE_VALUE_FUNC(int64);
        DECLARE_ARCHIVE_VALUE_FUNC(uint8);
        DECLARE_ARCHIVE_VALUE_FUNC(uint16);
        DECLARE_ARCHIVE_VALUE_FUNC(uint32);
        DECLARE_ARCHIVE_VALUE_FUNC(uint64);
        DECLARE_ARCHIVE_VALUE_FUNC(float32);
        DECLARE_ARCHIVE_VALUE_FUNC(float64);
        DECLARE_ARCHIVE_VALUE_FUNC(B1Object);
        DECLARE_ARCHIVE_VALUE_FUNC(B1String);
        DECLARE_ARCHIVE_VALUE_FUNC(std::vector<int8>);
        DECLARE_ARCHIVE_VALUE_FUNC(std::vector<int16>);
        DECLARE_ARCHIVE_VALUE_FUNC(std::vector<int32>);
        DECLARE_ARCHIVE_VALUE_FUNC(std::vector<int64>);
        DECLARE_ARCHIVE_VALUE_FUNC(std::vector<uint8>);
        DECLARE_ARCHIVE_VALUE_FUNC(std::vector<uint16>);
        DECLARE_ARCHIVE_VALUE_FUNC(std::vector<uint32>);
        DECLARE_ARCHIVE_VALUE_FUNC(std::vector<uint64>);
        DECLARE_ARCHIVE_VALUE_FUNC(std::vector<float32>);
        DECLARE_ARCHIVE_VALUE_FUNC(std::vector<float64>);
        DECLARE_ARCHIVE_VALUE_FUNC(std::vector<B1String>);
        template<typename T>
        void writeObjectArray(const B1String& key, const std::vector<T>& value);
        template<typename T>
        void readObjectArray(const B1String& key, std::vector<T>* value) const;
    public:
        void addArchive(const B1Archive& archive);
        void pushArchive(const B1Archive& archive); //  push_back
        bool popArchive(B1Archive* archive) const;  //  pop_front
        void addSubArchive(const B1String& key, const B1Archive& archive);
        bool getSubArchive(const B1String& key, B1Archive* archive) const;
        void getFrontArchive(B1Archive* archive) const;
        void toObject(B1Object* object) const;
        void fromObject(const B1Object& object);
        bool toString(B1String* string, bool pretty = false) const;
        bool fromString(const B1String& string);
    };
}   //  !BnD

#endif  // !_B1BASE_ARCHIVE_H
