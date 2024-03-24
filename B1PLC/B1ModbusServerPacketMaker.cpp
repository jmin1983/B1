//
// B1ModbusServerPacketMaker.cpp
//
// Library: B1PLC
// Package: B1PLC
// Module:  B1PLC
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1PLC.h"
#include "B1ModbusServerPacketMaker.h"
#include "B1ModbusProtocol.h"

#include <B1Base/B1Endian.h>

using namespace BnD;

std::vector<uint8> B1ModbusServerPacketMaker::makeReadDiscreteInputs(uint16 transactionID, uint16 beginAddress, uint16 count)
{
    std::vector<uint8> data;
    data.reserve(4);
    insertUint16(beginAddress, &data);
    insertUint16(count, &data);
    return makeFunctionData(transactionID, B1ModbusProtocol::FUNCTION_CODE_READ_DISCRETE_INPUTS, data);
}

std::vector<uint8> B1ModbusServerPacketMaker::makeReadCoil(uint16 transactionID, const std::vector<bool>& values)
{
    std::vector<uint8> data;
    uint8 tmpdata = 0;
    uint8 mask = 1;

    uint8 byteCount = static_cast<uint8>((values.size() + 7) / 8);
    data.reserve(2 + byteCount);
    data.push_back(byteCount);
    for (size_t i = 0; i < values.size(); ++i) {
        tmpdata += values[i] * mask;
        mask = mask << 1;
        if (i % 8 == 7) {
            data.push_back(tmpdata);
            tmpdata = 0;
            mask = 1;
        }
    }
    if (values.size() % 8 != 0) {
        data.push_back(tmpdata);
    }
    return makeFunctionData(transactionID, B1ModbusProtocol::FUNCTION_CODE_READ_COIL, data);
}

std::vector<uint8> B1ModbusServerPacketMaker::makeWriteSingleCoil(uint16 transactionID, uint16 address, uint16 value)
{
    std::vector<uint8> data;
    data.reserve(4);
    insertUint16(address, &data);
    insertUint16(value, &data);
    return makeFunctionData(transactionID, B1ModbusProtocol::FUNCTION_CODE_WRITE_SINGLE_COIL, data);
}

std::vector<uint8> B1ModbusServerPacketMaker::makeWriteMultipleCoils(uint16 transactionID, uint16 beginAddress, const std::vector<uint8>& values)
{
    std::vector<uint8> data;
    data.reserve(5 + values.size());
    insertUint16(beginAddress, &data);
    uint16 outputs = static_cast<uint16>(8 * values.size());
    insertUint16(outputs, &data);
    data.push_back(static_cast<uint8>(values.size()));
    data.insert(data.end(), values.begin(), values.end());
    return makeFunctionData(transactionID, B1ModbusProtocol::FUNCTION_CODE_WRITE_MULTIPLE_COILS, data);
}

std::vector<uint8> B1ModbusServerPacketMaker::makeReadInputRegister(uint16 transactionID, uint16 beginAddress, uint16 count)
{
    std::vector<uint8> data;
    data.reserve(4);
    insertUint16(beginAddress, &data);
    insertUint16(count, &data);
    return makeFunctionData(transactionID, B1ModbusProtocol::FUNCTION_CODE_READ_INPUT_REGISTER, data);
}

std::vector<uint8> B1ModbusServerPacketMaker::makeReadHoldingRegisters(uint16 transactionID, const std::vector<uint16>& values)
{
    std::vector<uint8> data;
    data.reserve(1 + 2 * values.size());
    data.push_back(static_cast<uint8>(2 * values.size()));
    for (uint16 value : values) {
        insertUint16(value, &data);
    }
    return makeFunctionData(transactionID, B1ModbusProtocol::FUNCTION_CODE_READ_HOLDING_REGISTERS, data);
}

std::vector<uint8> B1ModbusServerPacketMaker::makeWriteSingleRegister(uint16 transactionID, uint16 address, uint16 value)
{
    std::vector<uint8> data;
    data.reserve(4);
    insertUint16(address, &data);
    insertUint16(value, &data);
    return makeFunctionData(transactionID, B1ModbusProtocol::FUNCTION_CODE_WRITE_SINGLE_REGISTER, data);
}

std::vector<uint8> B1ModbusServerPacketMaker::makeWriteMultipleRegisters(uint16 transactionID, uint16 beginAddress, const std::vector<uint16>& values)
{
    std::vector<uint8> data;
    data.reserve(5 + values.size() * 2);
    insertUint16(beginAddress, &data);
    insertUint16(static_cast<uint16>(values.size()), &data);
    data.push_back(static_cast<uint8>(values.size() * 2));
    for (uint16 value : values) {
        insertUint16(value, &data);
    }
    return makeFunctionData(transactionID, B1ModbusProtocol::FUNCTION_CODE_WRITE_MULTIPLE_REGISTERS, data);
}

std::vector<uint8> B1ModbusServerPacketMaker::makeExceptions(uint16 transactionID, uint8 code, uint8 exceptionCode)
{
    return makeFunctionData(transactionID, B1ModbusProtocol::FUNCTION_CODE_ERROR_ADD_VALUE + code, std::vector<uint8>(1, exceptionCode));
}