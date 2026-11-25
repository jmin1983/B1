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
    if (data.size() != 4) {
        B1LOG("invalid data size: function[onFunctionReadDiscreteInputs], size[%u]", static_cast<uint32>(data.size()));
        return;
    }
    onServerFunctionReadDiscreteInputs(transactionID, toUint16(&data[0]), toUint16(&data[2]));
}

void B1ModbusServerPacketAnalyzer::onFunctionReadCoil(uint16 transactionID, const std::vector<uint8>& data)
{
    assert(data.size() == 4);
    if (data.size() != 4) {
        B1LOG("invalid data size: function[onFunctionReadCoil], size[%u]", static_cast<uint32>(data.size()));
        return;
    }
    onServerFunctionReadCoil(transactionID, toUint16(&data[0]), toUint16(&data[2]));
}

void B1ModbusServerPacketAnalyzer::onFunctionWriteSingleCoil(uint16 transactionID, const std::vector<uint8>& data)
{
    assert(data.size() == 4);
    if (data.size() != 4) {
        B1LOG("invalid data size: function[onFunctionWriteSingleCoil], size[%u]", static_cast<uint32>(data.size()));
        return;
    }
    onServerFunctionWriteSingleCoil(transactionID, toUint16(&data[0]), toUint16(&data[2]));
}

void B1ModbusServerPacketAnalyzer::onFunctionWriteMultipleCoils(uint16 transactionID, const std::vector<uint8>& data)
{
    assert(data.size() > 4);
    if (data.size() < 5) {
        B1LOG("invalid data size: function[onFunctionWriteMultipleCoils], size[%u]", static_cast<uint32>(data.size()));
        return;
    }
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
    if (data.size() != 4) {
        B1LOG("invalid data size: function[onFunctionReadInputRegister], size[%u]", static_cast<uint32>(data.size()));
        return;
    }
    onServerFunctionReadInputRegister(transactionID, toUint16(&data[0]), toUint16(&data[2]));
}

void B1ModbusServerPacketAnalyzer::onFunctionReadHoldingRegisters(uint16 transactionID, const std::vector<uint8>& data)
{
    assert(data.size() == 4);
    if (data.size() != 4) {
        B1LOG("invalid data size: function[onFunctionReadHoldingRegisters], size[%u]", static_cast<uint32>(data.size()));
        return;
    }
    onServerFunctionReadHoldingRegisters(transactionID, toUint16(&data[0]), toUint16(&data[2]));
}

void B1ModbusServerPacketAnalyzer::onFunctionWriteSingleRegister(uint16 transactionID, const std::vector<uint8>& data)
{
    assert(data.size() == 4);
    if (data.size() != 4) {
        B1LOG("invalid data size: function[onFunctionWriteSingleRegister], size[%u]", static_cast<uint32>(data.size()));
        return;
    }
    onServerFunctionWriteSingleRegister(transactionID, toUint16(&data[0]), toUint16(&data[2]));
}

void B1ModbusServerPacketAnalyzer::onFunctionWriteMultipleRegisters(uint16 transactionID, const std::vector<uint8>& data)
{
    assert(data.size() > 4);
    if (data.size() < 5 || (data.size() - 5) % 2 != 0) {
        B1LOG("invalid data size: function[onFunctionWriteMultipleRegisters], size[%u]", static_cast<uint32>(data.size()));
        return;
    }
    std::vector<uint16> values;
    values.reserve(toUint16(&data[2]));
    for (size_t i = 5; i < data.size(); ++i) {
        values.push_back(toUint16(&data[i++]));
    }
    onServerFunctionWriteMultipleRegisters(transactionID, toUint16(&data[0]), values);
}
