//
// B1ModbusClientPacketMaker.cpp
//
// Library: B1PLC
// Package: B1PLC
// Module:  B1PLC
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1PLC.h"
#include "B1ModbusClientPacketMaker.h"
#include "B1ModbusProtocol.h"

#include <B1Base/B1Endian.h>

using namespace BnD;

B1ModbusClientPacketMaker::B1ModbusClientPacketMaker()
    : _transactionID(0)
{
}

B1ModbusClientPacketMaker::~B1ModbusClientPacketMaker()
{
}

std::vector<uint8> B1ModbusClientPacketMaker::makeReadDiscreteInputs(uint16 beginAddress, uint16 count)
{
    std::vector<uint8> data;
    data.reserve(4);
    insertUint16(beginAddress, &data);
    insertUint16(count, &data);
    return makeFunctionData(_transactionID++, B1ModbusProtocol::FUNCTION_CODE_READ_DISCRETE_INPUTS, data);
}

std::vector<uint8> B1ModbusClientPacketMaker::makeReadCoil(uint16 beginAddress, uint16 count, uint16* transactionID)
{
    std::vector<uint8> data;
    data.reserve(4);
    insertUint16(beginAddress, &data);
    insertUint16(count, &data);
    if (transactionID) {
        *transactionID = _transactionID;
    }
    return makeFunctionData(_transactionID++, B1ModbusProtocol::FUNCTION_CODE_READ_COIL, data);
}

std::vector<uint8> B1ModbusClientPacketMaker::makeWriteSingleCoil(uint16 address, uint16 value)
{
    std::vector<uint8> data;
    data.reserve(4);
    insertUint16(address, &data);
    insertUint16(value, &data);
    return makeFunctionData(_transactionID++, B1ModbusProtocol::FUNCTION_CODE_WRITE_SINGLE_COIL, data);
}

std::vector<uint8> B1ModbusClientPacketMaker::makeWriteMultipleCoils(uint16 beginAddress, const std::vector<uint8>& values)
{
    std::vector<uint8> data;
    data.reserve(5 + values.size());
    insertUint16(beginAddress, &data);
    uint16 outputs = static_cast<uint16>(8 * values.size());
    insertUint16(outputs, &data);
    data.push_back(static_cast<uint8>(values.size()));
    data.insert(data.end(), values.begin(), values.end());
    return makeFunctionData(_transactionID++, B1ModbusProtocol::FUNCTION_CODE_WRITE_MULTIPLE_COILS, data);
}

std::vector<uint8> B1ModbusClientPacketMaker::makeReadInputRegister(uint16 beginAddress, uint16 count)
{
    std::vector<uint8> data;
    data.reserve(4);
    insertUint16(beginAddress, &data);
    insertUint16(count, &data);
    return makeFunctionData(_transactionID++, B1ModbusProtocol::FUNCTION_CODE_READ_INPUT_REGISTER, data);
}

std::vector<uint8> B1ModbusClientPacketMaker::makeReadHoldingRegisters(uint16 beginAddress, uint16 count, uint16* transactionID)
{
    std::vector<uint8> data;
    data.reserve(4);
    insertUint16(beginAddress, &data);
    insertUint16(count, &data);
    if (transactionID) {
        *transactionID = _transactionID;
    }
    return makeFunctionData(_transactionID++, B1ModbusProtocol::FUNCTION_CODE_READ_HOLDING_REGISTERS, data);
}

std::vector<uint8> B1ModbusClientPacketMaker::makeWriteSingleRegister(uint16 address, uint16 value)
{
    std::vector<uint8> data;
    data.reserve(4);
    insertUint16(address, &data);
    insertUint16(value, &data);
    return makeFunctionData(_transactionID++, B1ModbusProtocol::FUNCTION_CODE_WRITE_SINGLE_REGISTER, data);
}

std::vector<uint8> B1ModbusClientPacketMaker::makeWriteMultipleRegisters(uint16 beginAddress, const std::vector<uint16>& values)
{
    std::vector<uint8> data;
    data.reserve(5 + values.size() * 2);
    insertUint16(beginAddress, &data);
    insertUint16(static_cast<uint16>(values.size()), &data);
    data.push_back(static_cast<uint8>(values.size() * 2));
    for (uint16 value : values) {
        insertUint16(value, &data);
    }
    return makeFunctionData(_transactionID++, B1ModbusProtocol::FUNCTION_CODE_WRITE_MULTIPLE_REGISTERS, data);
}
