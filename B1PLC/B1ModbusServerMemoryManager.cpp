//
// B1ModbusServerMemoryManager.cpp
//
// Library: B1PLC
// Package: B1PLC
// Module:  B1PLC
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1PLC.h"
#include "B1ModbusServerMemoryManager.h"
#include "B1ModbusProtocol.h"

using namespace BnD;

B1ModbusServerMemoryManager::B1ModbusServerMemoryManager(uint16 holdingRegisterSize, uint16 coilSize)
{
    _holdingRegisters.resize(holdingRegisterSize, 0);
    _coils.resize(coilSize, false);
}

B1ModbusServerMemoryManager::~B1ModbusServerMemoryManager()
{
}

uint8 B1ModbusServerMemoryManager::setHoldingRegister(uint16 address, uint16 value)
{
    if (_holdingRegisters.size() <= address) {
        return B1ModbusProtocol::FUNCTION_CODE_ERROR_ILLEGAL_DATA_ADDRESS;
    }
    _holdingRegisters[address] = value;
    return 0;
}

uint8 B1ModbusServerMemoryManager::setHoldingRegisters(uint16 beginAddress, const std::vector<uint16>& values)
{
    if (values.size() > B1ModbusProtocol::FUNCTION_CODE_WRITE_MAX_QTY_OF_REGISTERS) {
        return B1ModbusProtocol::FUNCTION_CODE_ERROR_ILLEGAL_DATA_VALUE;
    }
    if (_holdingRegisters.size() < static_cast<size_t>(beginAddress + values.size())) {
        return B1ModbusProtocol::FUNCTION_CODE_ERROR_ILLEGAL_DATA_ADDRESS;
    }
    for (size_t i = 0; i < values.size(); ++i) {
        _holdingRegisters[beginAddress + i] = values[i];
    }
    return 0;
}

uint8 B1ModbusServerMemoryManager::getHoldingRegister(uint16 address, uint16* value)
{
    if (_holdingRegisters.size() <= address) {
        return B1ModbusProtocol::FUNCTION_CODE_ERROR_ILLEGAL_DATA_ADDRESS;
    }
    *value = _holdingRegisters[address];
    return 0;
}

uint8 B1ModbusServerMemoryManager::getHoldingRegisters(uint16 beginAddress, uint16 count, std::vector<uint16>* values)
{
    if (count > B1ModbusProtocol::FUNCTION_CODE_READ_MAX_QTY_OF_REGISTERS) {
        return B1ModbusProtocol::FUNCTION_CODE_ERROR_ILLEGAL_DATA_VALUE;
    }
    if (_holdingRegisters.size() < static_cast<size_t>(beginAddress + count)) {
        return B1ModbusProtocol::FUNCTION_CODE_ERROR_ILLEGAL_DATA_ADDRESS;
    }
    values->reserve(count);
    values->clear();
    for (int32 i = beginAddress; i < beginAddress + count; ++i) {
        values->push_back(_holdingRegisters[i]);
    }
    return 0;
}

uint8 B1ModbusServerMemoryManager::setCoil(uint16 address, bool value)
{
    if (_coils.size() <= address) {
        return B1ModbusProtocol::FUNCTION_CODE_ERROR_ILLEGAL_DATA_ADDRESS;
    }
    _coils[address] = value;
    return 0;
}

uint8 B1ModbusServerMemoryManager::setCoils(uint16 beginAddress, const std::vector<uint8>& values)
{
    std::vector<bool> bvalues;
    bvalues.reserve(values.size()*8);

    for (size_t i = 0; i < values.size(); ++i) {
        uint8 j = values[i];
        while (j != 0) {
            bvalues.push_back(j % 2);
            j = j >> 1;
        }
    }

    if (bvalues.size() > B1ModbusProtocol::FUNCTION_CODE_WRITE_MAX_QTY_OF_COILS) {
        return B1ModbusProtocol::FUNCTION_CODE_ERROR_ILLEGAL_DATA_VALUE;
    }
    if (_coils.size() < static_cast<size_t>(beginAddress + bvalues.size())) {
        return B1ModbusProtocol::FUNCTION_CODE_ERROR_ILLEGAL_DATA_ADDRESS;
    }

    for (size_t i = 0; i < bvalues.size(); ++i) {
        _coils[beginAddress + i] = bvalues[i];
    }
    return 0;
}

uint8 B1ModbusServerMemoryManager::getCoil(uint16 address, bool* value)
{
    if (_coils.size() <= address) {
        return B1ModbusProtocol::FUNCTION_CODE_ERROR_ILLEGAL_DATA_ADDRESS;
    }
    *value = _coils[address];
    return 0;
}

uint8 B1ModbusServerMemoryManager::getCoils(uint16 beginAddress, uint16 count, std::vector<bool>* values)
{
    if (count > B1ModbusProtocol::FUNCTION_CODE_READ_MAX_QTY_OF_COILS) {
        return B1ModbusProtocol::FUNCTION_CODE_ERROR_ILLEGAL_DATA_VALUE;
    }
    if (_coils.size() < static_cast<size_t>(beginAddress + count)) {
        return B1ModbusProtocol::FUNCTION_CODE_ERROR_ILLEGAL_DATA_ADDRESS;
    }
    values->reserve(count);
    values->clear();
    for (int32 i = beginAddress; i < beginAddress + count; ++i) {
        values->push_back(_coils[i]);
    }
    return 0;
}