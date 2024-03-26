//
// B1SECS2Data.h
//
// Library: B1SECS2
// Package: B1SECS2
// Module:  B1SECS2
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1SECS2_DATA_H
#define _B1SECS2_DATA_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <B1SECS2/B1SECS2Consts.h>

namespace BnD {
    class B1SECS2Data {
    public:
        B1SECS2Data(const B1SECS2Data& data);
        B1SECS2Data(B1SECS2Consts::DATA_FORMAT format, B1String&& label);
        virtual ~B1SECS2Data();
    protected:
        enum CONSTS {
            CONSTS_LENGTH_BUFFER_SIZE = 32,
        };
    protected:
        const B1SECS2Consts::DATA_FORMAT _format;
        B1String _label;
    protected:
        virtual bool implReadData(const uint8* data, uint32 dataLength, uint32 bodyLength, uint32* pos) = 0;
        virtual bool implWriteTo(std::vector<uint8>* result) const = 0;
        virtual void implToString(B1String* string) const = 0;
    protected:
        bool readBodyFormatExpected(const uint8* data, uint32 dataLength, uint32* pos, uint32* bodyLength);
        int8 getDataBodyLengthSize(uint32 dataLength) const;
        bool setMessageDataBodyLength(uint32 dataLength, std::vector<uint8>* result) const;
    public:
        virtual B1SECS2Data* clone() const = 0;
    public:
        B1SECS2Consts::DATA_FORMAT format() const { return _format; }
        bool isFormatList() const { return format() == B1SECS2Consts::DATA_FORMAT_LIST; }
        bool isFormatASCII() const { return format() == B1SECS2Consts::DATA_FORMAT_ASCII; }
        bool isFormatUint16() const { return format() == B1SECS2Consts::DATA_FORMAT_UINT16; }
        bool isFormatUint32() const { return format() == B1SECS2Consts::DATA_FORMAT_UINT32; }
        bool readDataOnly(const uint8* data, uint32 dataLength, uint32 bodyLength, uint32* pos);
        bool readFrom(const uint8* data, uint32 dataLength, uint32* pos);
        bool writeTo(std::vector<uint8>* result) const;
        B1String toString() const;
        const B1String& label() const { return _label; }
        void setLabel(B1String&& label) { _label = std::move(label); }
    public:
        B1SECS2Data& operator=(const B1SECS2Data& d) { assert(_format == d._format); return *this; }
    public:
        static bool readBodyFormat(const uint8* data, uint32 dataLength, uint32* pos, B1SECS2Consts::DATA_FORMAT* format, uint32* bodyLength);
        static B1SECS2Data* createFromUnknownFormat(const uint8* data, uint32 dataLength, uint32* pos);
    };
    class B1SECS2DataNullable : public B1SECS2Data {
    public:
        B1SECS2DataNullable(B1SECS2Consts::DATA_FORMAT format, B1String&& label);
    private:
        bool _nullData;
    protected:
        virtual bool implReadData(const uint8* data, uint32 dataLength, uint32 bodyLength, uint32* pos) override;   //  return false if not a null data.
    public:
        bool isNullData() const { return _nullData; }
    };

    class B1SECS2DataLIST : public B1SECS2Data {
    public:
        B1SECS2DataLIST(B1String&& label = "");
        B1SECS2DataLIST(uint32 count, B1String&& label = "");
    protected:
        uint32 _count;
    protected:
        virtual bool implReadData(const uint8* data, uint32 dataLength, uint32 bodyLength, uint32* pos) override;
        virtual bool implWriteTo(std::vector<uint8>* result) const override;
        virtual void implToString(B1String* string) const override;
        virtual B1SECS2DataLIST* clone() const override { return new B1SECS2DataLIST(*this); }
    public:
        uint32 count() const { return _count; }
        void setCount(uint32 count) { _count = count; }
    };
    class B1SECS2DataBIN : public B1SECS2Data {
    public:
        B1SECS2DataBIN(const char* label = "");
        B1SECS2DataBIN(uint8 singleValue, B1String&& label = "");
        B1SECS2DataBIN(const std::vector<uint8>& data, B1String&& label = "");
    protected:
        std::vector<uint8> _data;
    protected:
        virtual bool implReadData(const uint8* data, uint32 dataLength, uint32 bodyLength, uint32* pos) override;
        virtual bool implWriteTo(std::vector<uint8>* result) const override;
        virtual void implToString(B1String* string) const override;
        virtual B1SECS2DataBIN* clone() const override { return new B1SECS2DataBIN(*this); }
    public:
              std::vector<uint8>& data()       { return _data; }
        const std::vector<uint8>& data() const { return _data; }
        void setData(const std::vector<uint8>& d) { _data = d; }
        void setData(uint8 singleValue);
    };
    class B1SECS2DataBOOL : public B1SECS2Data {
    public:
        B1SECS2DataBOOL(const char* label = "");
        B1SECS2DataBOOL(bool data, B1String&& label = "");
    protected:
        bool _data;
    protected:
        virtual bool implReadData(const uint8* data, uint32 dataLength, uint32 bodyLength, uint32* pos) override;
        virtual bool implWriteTo(std::vector<uint8>* result) const override;
        virtual void implToString(B1String* string) const override;
        virtual B1SECS2DataBOOL* clone() const override { return new B1SECS2DataBOOL(*this); }
    public:
        bool data() const { return _data; }
        void setData(bool flag) { _data = flag; }
    };
    class B1SECS2DataASCII : public B1SECS2Data {
    public:
        B1SECS2DataASCII(const B1SECS2DataASCII& r);
        B1SECS2DataASCII(int32 limitSize = -1, B1String&& label = "");  //  no_limit if limitSize is less than zero
        B1SECS2DataASCII(const B1String& data, int32 limitSize = -1, B1String&& label = "");
        B1SECS2DataASCII(B1String&& data, int32 limitSize = -1, B1String&& label = "");
    protected:
        B1String _data;
        int32 _limitSize;
    protected:
        virtual bool implReadData(const uint8* data, uint32 dataLength, uint32 bodyLength, uint32* pos) override;
        virtual bool implWriteTo(std::vector<uint8>* result) const override;
        virtual void implToString(B1String* string) const override;
        virtual B1SECS2DataASCII* clone() const override { return new B1SECS2DataASCII(*this); }
    public:
        bool operator<(const B1SECS2DataASCII& s) const { return _data < s._data; }
    public:
        const B1String& data() const { return _data; }
        int32 limitSize() const { return _limitSize; }
        void setData(B1String&& d) { _data = std::move(d); }
        void setLimitSize(int32 limit) { _limitSize = limit; }
    };
    class B1SECS2DataUINT8 : public B1SECS2DataNullable {
    public:
        B1SECS2DataUINT8(const char* label = "");
        B1SECS2DataUINT8(uint8 data, B1String&& label = "");
    protected:
        uint8 _data;
    protected:
        virtual bool implReadData(const uint8* data, uint32 dataLength, uint32 bodyLength, uint32* pos) override;
        virtual bool implWriteTo(std::vector<uint8>* result) const override;
        virtual void implToString(B1String* string) const override;
        virtual B1SECS2DataUINT8* clone() const override { return new B1SECS2DataUINT8(*this); }
    public:
        bool operator<(const B1SECS2DataUINT8& s) const { return _data < s._data; }
    public:
        uint8 data() const { return _data; }
        void setData(uint8 d) { _data = d; }
    };
    class B1SECS2DataUINT16 : public B1SECS2DataNullable {
    public:
        B1SECS2DataUINT16(B1String&& label = "");
        B1SECS2DataUINT16(uint16 data, B1String&& label = "");
    protected:
        uint16 _data;
    protected:
        virtual bool implReadData(const uint8* data, uint32 dataLength, uint32 bodyLength, uint32* pos) override;
        virtual bool implWriteTo(std::vector<uint8>* result) const override;
        virtual void implToString(B1String* string) const override;
        virtual B1SECS2DataUINT16* clone() const override { return new B1SECS2DataUINT16(*this); }
    public:
        bool operator<(const B1SECS2DataUINT16& s) const { return _data < s._data; }
    public:
        uint16 data() const { return _data; }
        void setData(uint16 d) { _data = d; }
    };
    class B1SECS2DataUINT32 : public B1SECS2DataNullable {
    public:
        B1SECS2DataUINT32(B1String&& label = "");
        B1SECS2DataUINT32(uint32 data, B1String&& label = "");
    protected:
        uint32 _data;
    protected:
        virtual bool implReadData(const uint8* data, uint32 dataLength, uint32 bodyLength, uint32* pos) override;
        virtual bool implWriteTo(std::vector<uint8>* result) const override;
        virtual void implToString(B1String* string) const override;
        virtual B1SECS2DataUINT32* clone() const override { return new B1SECS2DataUINT32(*this); }
    public:
        bool operator<(const B1SECS2DataUINT32& s) const { return _data < s._data; }
    public:
        uint32 data() const { return _data; }
        void setData(uint32 d) { _data = d; }
    };

    typedef std::shared_ptr<B1SECS2Data> SPB1SECS2Data;
    typedef std::shared_ptr<const B1SECS2Data> SPCSECS2Data;
}   //  !BnD

#endif  // !_B1SECS2_DATA_H
