//
// B1ModbusServerPacketAnalyzer.cpp
//
// Library: B1PLC
// Package: B1PLC
// Module:  B1PLC
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1PLC.h"
#include "B1ModbusServerPacketAnalyzer.h"

using namespace BnD;

void B1ModbusServerPacketAnalyzer::onFunctionReadDiscreteInputs(uint16 transactionID, const std::vector<uint8>& data)
{
    assert(data.size() == 4);
    onServerFunctionReadDiscreteInputs(transactionID, toUint16(&data[0]), toUint16(&data[2]));
}

void B1ModbusServerPacketAnalyzer::onFunctionReadCoil(uint16 transactionID, const std::vector<uint8>& data)
{
    assert(data.size() == 4);
    onServerFunctionReadCoil(transactionID, toUint16(&data[0]), toUint16(&data[2]));
}

void B1ModbusServerPacketAnalyzer::onFunctionWriteSingleCoil(uint16 transactionID, const std::vector<uint8>& data)
{
    assert(data.size() == 4);
    onServerFunctionWriteSingleCoil(transactionID, toUint16(&data[0]), toUint16(&data[2]));
}

void B1ModbusServerPacketAnalyzer::onFunctionWriteMultipleCoils(uint16 transactionID, const std::vector<uint8>& data)
{
    assert(data.size() > 4);
    std::vector<uint8> values;
    values.reserve(data[4]);
    for (size_t i = 5; i < data.size(); ++i) {
        values.push_back(data[i]);
    }
    onServerFunctionWriteMultipleCoils(transactionID, toUint16(&data[0]), values);
}

void B1ModbusServerPacketAnalyzer::onFunctionReadInputRegister(uint16 transactionID, const std::vector<uint8>& data)
{
    assert(data.size() == 4);
    onServerFunctionReadInputRegister(transactionID, toUint16(&data[0]), toUint16(&data[2]));
}

void B1ModbusServerPacketAnalyzer::onFunctionReadHoldingRegisters(uint16 transactionID, const std::vector<uint8>& data)
{
    assert(data.size() == 4);
    onServerFunctionReadHoldingRegisters(transactionID, toUint16(&data[0]), toUint16(&data[2]));
}

void B1ModbusServerPacketAnalyzer::onFunctionWriteSingleRegister(uint16 transactionID, const std::vector<uint8>& data)
{
    assert(data.size() == 4);
    onServerFunctionWriteSingleRegister(transactionID, toUint16(&data[0]), toUint16(&data[2]));
}

void B1ModbusServerPacketAnalyzer::onFunctionWriteMultipleRegisters(uint16 transactionID, const std::vector<uint8>& data)
{
    assert(data.size() > 4);
    std::vector<uint16> values;
    values.reserve(toUint16(&data[2]));
    for (size_t i = 5; i < data.size(); ++i) {
        values.push_back(toUint16(&data[i++]));
    }
    onServerFunctionWriteMultipleRegisters(transactionID, toUint16(&data[0]), values);
}
