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
    onClientFunctionReadDiscreteInputs(std::vector<uint8>(++data.begin(), data.end()));
}

void B1ModbusClientPacketAnalyzer::onFunctionReadCoil(uint16 transactionID, const std::vector<uint8>& data)
{
    assert(data.front() == std::vector<uint8>(++data.begin(), data.end()).size());
    onClientFunctionReadCoil(transactionID, std::vector<uint8>(++data.begin(), data.end()));
}

void B1ModbusClientPacketAnalyzer::onFunctionWriteSingleCoil(uint16 transactionID, const std::vector<uint8>& data)
{
    assert(data.size() == 4);
    onClientFunctionWriteSingleCoil(toUint16(&data[0]), toUint16(&data[2]));
}

void B1ModbusClientPacketAnalyzer::onFunctionWriteMultipleCoils(uint16 transactionID, const std::vector<uint8>& data)
{
    assert(data.size() == 4);
    onClientFunctionWriteMultipleCoils(toUint16(&data[0]), toUint16(&data[2]));
}

void B1ModbusClientPacketAnalyzer::onFunctionReadInputRegister(uint16 transactionID, const std::vector<uint8>& data)
{
    assert(data.front() % 2 == 0);
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
    onClientFunctionWriteSingleRegister(toUint16(&data[0]), toUint16(&data[2]));
}

void B1ModbusClientPacketAnalyzer::onFunctionWriteMultipleRegisters(uint16 transactionID, const std::vector<uint8>& data)
{
    assert(data.size() == 4);
    onClientFunctionWriteMultipleRegisters(toUint16(&data[0]), toUint16(&data[2]));
}
