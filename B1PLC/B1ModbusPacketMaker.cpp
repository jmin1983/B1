//
// B1ModbusPacketMaker.cpp
//
// Library: B1PLC
// Package: B1PLC
// Module:  B1PLC
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1PLC.h"
#include "B1ModbusPacketMaker.h"
#include "B1ModbusProtocol.h"

#include <B1Base/B1Endian.h>

using namespace BnD;

B1ModbusPacketMaker::B1ModbusPacketMaker()
{
}

B1ModbusPacketMaker::~B1ModbusPacketMaker()
{
}

void B1ModbusPacketMaker::insertUint16(uint16 value, std::vector<uint8>* buffer)
{
    buffer->push_back((uint8)(value >> 8));
    buffer->push_back((uint8)(value));
}

void B1ModbusPacketMaker::insertMBAP(uint16 transactionID, size_t dataSize, std::vector<uint8>* buffer)
{
    uint16 length = static_cast<uint16>(dataSize + 1);  //  dataSize + unit_id_size.
    buffer->reserve(sizeof(B1ModbusProtocol::MBAP) + length);
    insertUint16(transactionID, buffer);
    insertUint16(0, buffer);    //  0 for Modbus/TCP.
    insertUint16(length, buffer);
    buffer->push_back(1);       //  1 for Modbus/TCP.
}

std::vector<uint8> B1ModbusPacketMaker::makeFunctionData(uint16 transactionID, uint8 code, const std::vector<uint8>& data)
{
    std::vector<uint8> buffer;
    insertMBAP(transactionID, data.size() + 1, &buffer);    //  data.size() + sizeof(code).
    buffer.push_back(code);
    if (data.empty() != true) {
        buffer.insert(buffer.end(), data.begin(), data.end());
    }
    return buffer;
}
