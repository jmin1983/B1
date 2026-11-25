//
// B1ModbusClientPacketAnalyzer.cpp
//
// Library: B1PLC
// Package: B1PLC
// Module:  B1PLC
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1PLC.h"
#include "B1ModbusClientPacketAnalyzer.h"

using namespace BnD;

void B1ModbusClientPacketAnalyzer::onFunctionReadDiscreteInputs(uint16 transactionID, const std::vector<uint8>& data)
{
    assert(data.front() == std::vector<uint8>(++data.begin(), data.end()).size());
    if (data.front() != data.size() - 1) {
        B1LOG("invalid data size: function[onFunctionReadDiscreteInputs], size[%u]", static_cast<uint32>(data.size()));
        return;
    }
    onClientFunctionReadDiscreteInputs(std::vector<uint8>(++data.begin(), data.end()));
}

void B1ModbusClientPacketAnalyzer::onFunctionReadCoil(uint16 transactionID, const std::vector<uint8>& data)
{
    assert(data.front() == std::vector<uint8>(++data.begin(), data.end()).size());
    if (data.front() != data.size() - 1) {
        B1LOG("invalid data size: function[onFunctionReadCoil], size[%u]", static_cast<uint32>(data.size()));
        return;
    }
    onClientFunctionReadCoil(transactionID, std::vector<uint8>(++data.begin(), data.end()));
}

void B1ModbusClientPacketAnalyzer::onFunctionWriteSingleCoil(uint16 transactionID, const std::vector<uint8>& data)
{
    assert(data.size() == 4);
    if (data.size() != 4) {
        B1LOG("invalid data size: function[onFunctionWriteSingleCoil], size[%u]", static_cast<uint32>(data.size()));
        return;
    }
    onClientFunctionWriteSingleCoil(toUint16(&data[0]), toUint16(&data[2]));
}

void B1ModbusClientPacketAnalyzer::onFunctionWriteMultipleCoils(uint16 transactionID, const std::vector<uint8>& data)
{
    assert(data.size() == 4);
    if (data.size() != 4) {
        B1LOG("invalid data size: function[onFunctionWriteMultipleCoils], size[%u]", static_cast<uint32>(data.size()));
        return;
    }
    onClientFunctionWriteMultipleCoils(toUint16(&data[0]), toUint16(&data[2]));
}

void B1ModbusClientPacketAnalyzer::onFunctionReadInputRegister(uint16 transactionID, const std::vector<uint8>& data)
{
    assert(data.front() % 2 == 0);
    if (data.front() % 2 != 0 || data.front() != data.size() - 1) {
        B1LOG("invalid data size: function[onFunctionReadInputRegister], size[%u]", static_cast<uint32>(data.size()));
        return;
    }
    size_t expectedSize = data.front() / 2;
    std::vector<uint16> registerData;
    registerData.reserve(expectedSize);
    for (size_t i = 1; i < data.size(); ++i) {
        registerData.push_back(toUint16(&data[i++]));
    }
    assert(expectedSize == registerData.size());
    onClientFunctionReadInputRegister(registerData);
}

void B1ModbusClientPacketAnalyzer::onFunctionReadHoldingRegisters(uint16 transactionID, const std::vector<uint8>& data)
{
    assert(data.front() % 2 == 0);
    if (data.front() % 2 != 0 || data.front() != data.size() - 1) {
        B1LOG("invalid data size: function[onFunctionReadHoldingRegisters], size[%u]", static_cast<uint32>(data.size()));
        return;
    }
    size_t expectedSize = data.front() / 2;
    std::vector<uint16> registerData;
    registerData.reserve(expectedSize);
    for (size_t i = 1; i < data.size(); ++i) {
        registerData.push_back(toUint16(&data[i++]));
    }
    assert(expectedSize == registerData.size());
    onClientFunctionReadHoldingRegisters(transactionID, registerData);
}

void B1ModbusClientPacketAnalyzer::onFunctionWriteSingleRegister(uint16 transactionID, const std::vector<uint8>& data)
{
    assert(data.size() == 4);
    if (data.size() != 4) {
        B1LOG("invalid data size: function[onFunctionWriteSingleRegister], size[%u]", static_cast<uint32>(data.size()));
        return;
    }
    onClientFunctionWriteSingleRegister(toUint16(&data[0]), toUint16(&data[2]));
}

void B1ModbusClientPacketAnalyzer::onFunctionWriteMultipleRegisters(uint16 transactionID, const std::vector<uint8>& data)
{
    assert(data.size() == 4);
    if (data.size() != 4) {
        B1LOG("invalid data size: function[onFunctionWriteMultipleRegisters], size[%u]", static_cast<uint32>(data.size()));
        return;
    }
    onClientFunctionWriteMultipleRegisters(toUint16(&data[0]), toUint16(&data[2]));
}
