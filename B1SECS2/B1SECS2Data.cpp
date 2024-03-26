//
// B1SECS2Data.cpp
//
// Library: B1SECS2
// Package: B1SECS2
// Module:  B1SECS2
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1SECS2.h"
#include "B1SECS2Data.h"

#include <B1Base/B1Endian.h>

using namespace BnD;
using namespace B1SECS2Consts;

B1SECS2Data::B1SECS2Data(const B1SECS2Data& data)
    : _format(data._format)
    , _label(std::move(data._label.copy()))
{
}

B1SECS2Data::B1SECS2Data(DATA_FORMAT format, B1String&& label)
    : _format(format)
    , _label(std::move(label))
{
}

B1SECS2Data::~B1SECS2Data()
{
}

bool B1SECS2Data::readBodyFormatExpected(const uint8* data, uint32 dataLength, uint32* pos, uint32* bodyLength)
{
    DATA_FORMAT format;
    if (B1SECS2Data::readBodyFormat(data, dataLength, pos, &format, bodyLength) != true)
        return false;
    if (format != _format) {
        B1LOG("not a Expected Format: expected[%o], real[%o]", _format, format);
        return false;
    }
    return true;
}

int8 B1SECS2Data::getDataBodyLengthSize(uint32 dataLength) const
{
    if (dataLength < 256)
        return 1;
    else if (dataLength < 65536)
        return 2;
    else if (dataLength < 16777215)
        return 3;
    else {
        B1LOG("body length too long");
        return -1;
    }
}

bool B1SECS2Data::setMessageDataBodyLength(uint32 dataLength, std::vector<uint8>* result) const
{
    int8 dataBodyLengthSize = getDataBodyLengthSize(dataLength);
    if (dataBodyLengthSize < 0) {
        B1LOG("dataBodyLengthSize error");
        return false;
    }
    result->push_back((_format << 2) | dataBodyLengthSize);
    if (dataBodyLengthSize == 1) {
        uint8 temp = static_cast<uint8>(dataLength);
        result->push_back(temp);
    }
    else if (dataBodyLengthSize == 2) {
        uint16 temp = static_cast<uint16>(dataLength);
        temp = TO_UINT16_FOR_NETWORK(temp);
        result->insert(result->end(), (uint8*)&temp + 0, (uint8*)&temp + 2);
    }
    else if (dataBodyLengthSize == 3) {
        uint32 temp = TO_UINT32_FOR_NETWORK(dataLength);
        result->insert(result->end(), (uint8*)&temp + 0, (uint8*)&temp + 4);
    }
    return true;
}

bool B1SECS2Data::readDataOnly(const uint8* data, uint32 dataLength, uint32 bodyLength, uint32* pos)
{
    if (implReadData(data, dataLength, bodyLength, pos) != true)
        return false;
    return true;
}

bool B1SECS2Data::readFrom(const uint8* data, uint32 dataLength, uint32* pos)
{
    uint32 bodyLength = 0;
    if (readBodyFormatExpected(data, dataLength, pos, &bodyLength) != true)
        return false;
    if (*pos + bodyLength > dataLength) {
        B1LOG("bodyLength over dataLength: pos_need[%u], dataLength[%u]", *pos + bodyLength, dataLength);
        return false;
    }
    if (implReadData(data, dataLength, bodyLength, pos) != true)
        return false;
    return true;
}

bool B1SECS2Data::writeTo(std::vector<uint8>* result) const
{
    return implWriteTo(result);
}

B1String B1SECS2Data::toString() const
{
    B1String string = _label.isEmpty() ? "ANY" : _label.copy();
    implToString(&string);
    return string;
}

bool B1SECS2Data::readBodyFormat(const uint8* data, uint32 dataLength, uint32* pos, DATA_FORMAT* format, uint32* bodyLength)
{
    if (*pos >= dataLength) {
        v2log("dataLength error");
        return false;
    }
    *format = static_cast<DATA_FORMAT>(data[*pos] >> 2);
    int8 bodyLengthSize = data[*pos] & 0x3;
    (*pos)++;
    if (*pos + bodyLengthSize > dataLength) {
        v2log("bodyLengthSize over");
        return false;
    }
    if (bodyLengthSize == 1)
        *bodyLength = data[*pos];
    else if (bodyLengthSize == 2) {
        uint16 temp = 0;
        memcpy(&temp, &data[*pos], bodyLengthSize);
        temp = TO_UINT16_FOR_NETWORK(temp);
        *bodyLength = temp;
    }
    else if (bodyLengthSize == 3) {
        uint32 temp = 0;
        memcpy(&temp, &data[*pos], bodyLengthSize);
        temp = TO_UINT32_FOR_NETWORK(temp);
        *bodyLength = temp;
    }
    else {
        v2log("bodyLengthSize error");
        return false;
    }
    (*pos) += bodyLengthSize;
    return true;
}

B1SECS2Data* B1SECS2Data::createFromUnknownFormat(const uint8* data, uint32 dataLength, uint32* pos)
{
    DATA_FORMAT format;
    uint32 bodyLength = 0;
    if (B1SECS2Data::readBodyFormat(data, dataLength, pos, &format, &bodyLength) != true)
        return NULL;
    B1SECS2Data* secs2Data = NULL;
    switch (format) {
        case DATA_FORMAT_LIST:
            secs2Data = new B1SECS2DataLIST();
            break;
        case DATA_FORMAT_BIN:
            secs2Data = new B1SECS2DataBIN();
            break;
        case DATA_FORMAT_BOOL:
            secs2Data = new B1SECS2DataBOOL();
            break;
        case DATA_FORMAT_ASCII:
            secs2Data = new B1SECS2DataASCII();
            break;
        case DATA_FORMAT_UINT8:
            secs2Data = new B1SECS2DataUINT8();
            break;
        case DATA_FORMAT_UINT16:
            secs2Data = new B1SECS2DataUINT16();
            break;
        case DATA_FORMAT_UINT32:
            secs2Data = new B1SECS2DataUINT32();
            break;
        default:
            return NULL;
    }
    if (secs2Data->readDataOnly(data, dataLength, bodyLength, pos) != true) {
        delete secs2Data;
        return NULL;
    }
    return secs2Data;
}

B1SECS2DataNullable::B1SECS2DataNullable(B1SECS2Consts::DATA_FORMAT format, B1String&& label)
    : B1SECS2Data(format, std::move(label))
    , _nullData(false)
{
}

bool B1SECS2DataNullable::implReadData(const uint8* data, uint32 dataLength, uint32 bodyLength, uint32* pos)
{
    if (0 == bodyLength) {
        _nullData = true;
        return true;
    }
    return false;
}

B1SECS2DataLIST::B1SECS2DataLIST(B1String&& label)
    : B1SECS2Data(DATA_FORMAT_LIST, std::move(label))
    , _count(0)
{
}

B1SECS2DataLIST::B1SECS2DataLIST(uint32 count, B1String&& label)
    : B1SECS2Data(DATA_FORMAT_LIST, std::move(label))
    , _count(count)
{
}

bool B1SECS2DataLIST::implReadData(const uint8* data, uint32 dataLength, uint32 bodyLength, uint32* pos)
{
    _count = bodyLength;
    return true;
}

bool B1SECS2DataLIST::implWriteTo(std::vector<uint8>* result) const
{
    result->reserve(_count + CONSTS_LENGTH_BUFFER_SIZE);
    if (setMessageDataBodyLength(_count, result) != true)
        return false;
    return true;
}

void B1SECS2DataLIST::implToString(B1String* string) const
{
    string->appendf(" <L %u", _count);
}

B1SECS2DataBIN::B1SECS2DataBIN(const char* label)
    : B1SECS2Data(DATA_FORMAT_BIN, label)
    , _data()
{
}

B1SECS2DataBIN::B1SECS2DataBIN(uint8 singleValue, B1String&& label)
    : B1SECS2Data(DATA_FORMAT_BIN, std::move(label))
    , _data(1)
{
    _data[0] = singleValue;
}

B1SECS2DataBIN::B1SECS2DataBIN(const std::vector<uint8>& data, B1String&& label)
    : B1SECS2Data(DATA_FORMAT_BIN, std::move(label))
    , _data(data)
{
}

bool B1SECS2DataBIN::implReadData(const uint8* data, uint32 dataLength, uint32 bodyLength, uint32* pos)
{
    std::vector<uint8>(data + *pos, data + *pos + bodyLength).swap(_data);
    *pos += bodyLength;
    return true;
}

bool B1SECS2DataBIN::implWriteTo(std::vector<uint8>* result) const
{
    uint32 count = static_cast<uint32>(_data.size());
    result->reserve(count + CONSTS_LENGTH_BUFFER_SIZE);
    if (setMessageDataBodyLength(count, result) != true)
        return false;
    result->insert(result->end(), _data.begin(), _data.end());
    return true;
}

void B1SECS2DataBIN::implToString(B1String* string) const
{
    string->appendf(" B [%d]", _data.empty() ? -1 : _data.front());
}

void B1SECS2DataBIN::setData(uint8 singleValue)
{
    _data.resize(1);
    _data[0] = singleValue;
}

B1SECS2DataBOOL::B1SECS2DataBOOL(const char* label)
    : B1SECS2Data(DATA_FORMAT_BOOL, label)
    , _data(false)
{
}

B1SECS2DataBOOL::B1SECS2DataBOOL(bool data, B1String&& label)
    : B1SECS2Data(DATA_FORMAT_BOOL, std::move(label))
    , _data(data)
{
}

bool B1SECS2DataBOOL::implReadData(const uint8* data, uint32 dataLength, uint32 bodyLength, uint32* pos)
{
    _data = data[*pos];
    *pos += bodyLength;
    return true;
}

bool B1SECS2DataBOOL::implWriteTo(std::vector<uint8>* result) const
{
    const uint32 dataSize = 1;
    result->reserve(dataSize + CONSTS_LENGTH_BUFFER_SIZE);
    if (setMessageDataBodyLength(dataSize, result) != true)
        return false;
    result->push_back(_data ? 1 : 0);
    return true;
}

void B1SECS2DataBOOL::implToString(B1String* string) const
{
    string->appendf(" BOOLEAN [%d]", _data ? 1 : 0);
}

B1SECS2DataASCII::B1SECS2DataASCII(const B1SECS2DataASCII& r)
    : B1SECS2Data(r)
    , _data(r._data.copy())
    , _limitSize(r._limitSize)
{
}

B1SECS2DataASCII::B1SECS2DataASCII(int32 limitSize, B1String&& label)
    : B1SECS2Data(DATA_FORMAT_ASCII, std::move(label))
    , _data()
    , _limitSize(limitSize)
{
}

B1SECS2DataASCII::B1SECS2DataASCII(const B1String& data, int32 limitSize, B1String&& label)
    : B1SECS2Data(DATA_FORMAT_ASCII, std::move(label))
    , _data(data.copy())
    , _limitSize(limitSize)
{
}

B1SECS2DataASCII::B1SECS2DataASCII(B1String&& data, int32 limitSize, B1String&& label)
    : B1SECS2Data(DATA_FORMAT_ASCII, std::move(label))
    , _data(std::move(data))
    , _limitSize(limitSize)
{
}

bool B1SECS2DataASCII::implReadData(const uint8* data, uint32 dataLength, uint32 bodyLength, uint32* pos)
{
    std::vector<char> ascii(bodyLength + 1, 0);
    memcpy(&ascii[0], &data[*pos], bodyLength);
    _data.from(&ascii[0]);
    *pos += bodyLength;
    return true;
}

bool B1SECS2DataASCII::implWriteTo(std::vector<uint8>* result) const
{
    std::vector<uint8> dataString;
    if (_limitSize < 0)
        dataString.insert(dataString.end(), _data.cString(), _data.cString() + _data.length());
    else
        dataString.insert(dataString.end(), _data.cString(), _data.cString() + std::min<uint32>(_data.length(), _limitSize));
    result->reserve(dataString.size() + CONSTS_LENGTH_BUFFER_SIZE);
    if (setMessageDataBodyLength(static_cast<uint32>(dataString.size()), result) != true)
        return false;
    result->insert(result->end(), dataString.begin(), dataString.end());
    return true;
}

void B1SECS2DataASCII::implToString(B1String* string) const
{
    string->appendf(" A [%s]", _data.cString());
}

B1SECS2DataUINT8::B1SECS2DataUINT8(const char* label)
    : B1SECS2DataNullable(DATA_FORMAT_UINT8, label)
    , _data(0)
{
}

B1SECS2DataUINT8::B1SECS2DataUINT8(uint8 data, B1String&& label)
    : B1SECS2DataNullable(DATA_FORMAT_UINT8, std::move(label))
    , _data(data)
{
}

bool B1SECS2DataUINT8::implReadData(const uint8* data, uint32 dataLength, uint32 bodyLength, uint32* pos)
{
    if (B1SECS2DataNullable::implReadData(data, dataLength, bodyLength, pos)) {
        return true;
    }
    const uint32 dataSize = 1;
    if (bodyLength != dataSize) {
        B1LOG("bodyLength is not uint32: bodyLength[%u]", bodyLength);
        return false;
    }
    _data = data[*pos];
    *pos += bodyLength;
    return true;
}

bool B1SECS2DataUINT8::implWriteTo(std::vector<uint8>* result) const
{
    const uint32 dataSize = 1;
    result->reserve(dataSize + CONSTS_LENGTH_BUFFER_SIZE);
    if (setMessageDataBodyLength(dataSize, result) != true)
        return false;
    result->push_back(_data);
    return true;
}

void B1SECS2DataUINT8::implToString(B1String* string) const
{
    string->appendf(" U1 [%u]", _data);
}

B1SECS2DataUINT16::B1SECS2DataUINT16(B1String&& label)
    : B1SECS2DataNullable(DATA_FORMAT_UINT16, std::move(label))
    , _data(0)
{
}

B1SECS2DataUINT16::B1SECS2DataUINT16(uint16 data, B1String&& label)
    : B1SECS2DataNullable(DATA_FORMAT_UINT16, std::move(label))
    , _data(data)
{
}

bool B1SECS2DataUINT16::implReadData(const uint8* data, uint32 dataLength, uint32 bodyLength, uint32* pos)
{
    if (B1SECS2DataNullable::implReadData(data, dataLength, bodyLength, pos)) {
        return true;
    }
    const uint32 dataSize = 2;
    if (bodyLength != dataSize) {
        B1LOG("bodyLength is not uint32: bodyLength[%u]", bodyLength);
        return false;
    }
    uint16 temp = 0;
    memcpy(&temp, &data[*pos], bodyLength);
    _data = TO_UINT16_FOR_NETWORK(temp);
    *pos += bodyLength;
    return true;
}

bool B1SECS2DataUINT16::implWriteTo(std::vector<uint8>* result) const
{
    const uint32 dataSize = 2;
    result->reserve(dataSize + CONSTS_LENGTH_BUFFER_SIZE);
    if (setMessageDataBodyLength(dataSize, result) != true)
        return false;
    uint16 data = TO_UINT16_FOR_NETWORK(_data);
    result->insert(result->end(), (uint8*)&data, (uint8*)&data + dataSize);
    return true;
}

void B1SECS2DataUINT16::implToString(B1String* string) const
{
    string->appendf(" U2 [%u]", _data);
}

B1SECS2DataUINT32::B1SECS2DataUINT32(B1String&& label)
    : B1SECS2DataNullable(DATA_FORMAT_UINT32, std::move(label))
    , _data(0)
{
}

B1SECS2DataUINT32::B1SECS2DataUINT32(uint32 data, B1String&& label)
    : B1SECS2DataNullable(DATA_FORMAT_UINT32, std::move(label))
    , _data(data)
{
}

bool B1SECS2DataUINT32::implReadData(const uint8* data, uint32 dataLength, uint32 bodyLength, uint32* pos)
{
    if (B1SECS2DataNullable::implReadData(data, dataLength, bodyLength, pos)) {
        return true;
    }
    const uint32 dataSize = 4;
    if (bodyLength != dataSize) {
        B1LOG("bodyLength is not uint32: bodyLength[%u]", bodyLength);
        return false;
    }
    uint32 temp = 0;
    memcpy(&temp, &data[*pos], bodyLength);
    _data = TO_UINT32_FOR_NETWORK(temp);
    *pos += bodyLength;
    return true;
}

bool B1SECS2DataUINT32::implWriteTo(std::vector<uint8>* result) const
{
    const uint32 dataSize = 4;
    result->reserve(dataSize + CONSTS_LENGTH_BUFFER_SIZE);
    if (setMessageDataBodyLength(dataSize, result) != true)
        return false;
    uint32 data = TO_UINT32_FOR_NETWORK(_data);
    result->insert(result->end(), (uint8*)&data, (uint8*)&data + dataSize);
    return true;
}

void B1SECS2DataUINT32::implToString(B1String* string) const
{
    string->appendf(" U4 [%u]", _data);
}
