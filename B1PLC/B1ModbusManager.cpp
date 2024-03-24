//
// B1ModbusManager.cpp
//
// Library: B1PLC
// Package: B1PLC
// Module:  B1PLC
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1PLC.h"
#include "B1ModbusManager.h"
#include "B1ModbusClient.h"
#include "B1ModbusProtocol.h"

#if defined(_DEBUG)
#define PRINT_UPDATED_VALUES 1
#else
#define PRINT_UPDATED_VALUES 0
#endif

#include <B1Base/B1Lock.h>
#include <B1Base/B1Thread.h>

using namespace BnD;

B1ModbusManager::B1ModbusManager()
    : _sendLock(new B1Lock())
    , _sendData()
    , _coilSentTransactionIDs()
    , _registerSentTransactionIDs()
{
}

B1ModbusManager::~B1ModbusManager()
{
}

bool B1ModbusManager::sendUpdateMemoryValue()
{
    {
        const uint16 total = static_cast<uint16>(_coils.size());
        int32 left = total;
        uint16 beginAddress = 0;
        while (left > 0) {
            uint16 transactionID = 0;
            uint16 count = std::min<uint16>(left, B1ModbusProtocol::FUNCTION_CODE_READ_MAX_QTY_OF_COILS);
            if (sendReadCoil(beginAddress, count, &transactionID) != true) {
                return false;
            }
            if (_coilSentTransactionIDs.empty() != true) {
                _coilSentTransactionIDs[beginAddress / B1ModbusProtocol::FUNCTION_CODE_READ_MAX_QTY_OF_COILS] = transactionID;
            }
            B1Thread::sleep(CONSTS_SEND_DATA_INTERVAL);
            left -= count;
            beginAddress += count;
        }
    }
    {
        const uint16 total = static_cast<uint16>(_holdingRegisters.size());
        int32 left = total;
        uint16 beginAddress = 0;
        while (left > 0) {
            uint16 transactionID = 0;
            uint16 count = std::min<uint16>(left, B1ModbusProtocol::FUNCTION_CODE_READ_MAX_QTY_OF_REGISTERS);
            if (sendReadHoldingRegisters(beginAddress, count, &transactionID) != true) {
                return false;
            }
            if (_registerSentTransactionIDs.empty() != true) {
                _registerSentTransactionIDs[beginAddress / B1ModbusProtocol::FUNCTION_CODE_READ_MAX_QTY_OF_REGISTERS] = transactionID;
            }
            B1Thread::sleep(CONSTS_SEND_DATA_INTERVAL);
            left -= count;
            beginAddress += count;
        }
    }
    return true;
}

bool B1ModbusManager::sendReadCoil(uint16 beginAddress, uint16 count, uint16* transactionID)
{
    return _client ? _client->sendReadCoil(beginAddress, count, transactionID) : false;
}

bool B1ModbusManager::sendWriteSingleCoil(const SendDataBool& data)
{
    return _client ? _client->sendWriteSingleCoil(data._beginAddress, data._set ? 0xFF00 : 0x00) : false;
}

bool B1ModbusManager::sendWriteMultipleCoils(const SendDataBools& data)
{
    if (_client == NULL) {
        return false;
    }
    const std::vector<bool>& coils = data._data;
    size_t valueSize = coils.size() / 8;
    if (coils.size() % 8 != 0) {
        valueSize++;
    }
    std::vector<uint8> values(valueSize, 0);
    for (size_t i = 0; i < coils.size(); ++i) {
        values[i / 8] |= coils[i] << (i % 8);
    }
    return _client->sendWriteMultipleCoils(data._beginAddress, values);
}

bool B1ModbusManager::sendReadHoldingRegisters(uint16 beginAddress, uint16 count, uint16* transactionID)
{
    return _client ? _client->sendReadHoldingRegisters(beginAddress, count, transactionID) : false;
}

bool B1ModbusManager::sendWriteSingleRegister(const SendDataValue& data)
{
    return _client ? _client->sendWriteSingleRegister(data._beginAddress, data._value) : false;
}

bool B1ModbusManager::sendWriteMultipleRegisters(const SendDataUint16s& data)
{
    return _client ? _client->sendWriteMultipleRegisters(data._beginAddress, data._data) : false;
}

int32 B1ModbusManager::foundTransactionIDIndex(uint16 transactionID, const std::vector<uint16>& transactionIDs)
{
    for (int32 i = 0; i < static_cast<int32>(transactionIDs.size()); ++i) {
        if (transactionIDs[i] == transactionID) {
            return i;
        }
    }
    return  -1;
}

void B1ModbusManager::onFunctionReadDiscreteInputs(const std::vector<uint8>& data)
{
}

void B1ModbusManager::onFunctionReadCoil(uint16 transactionID, const std::vector<uint8>& data)
{
#if (PRINT_UPDATED_VALUES)
    std::vector<bool> coils = _coils;
#endif

    uint16 beginAddress = 0;
    if (_coilSentTransactionIDs.empty() != true) {
        int32 index = foundTransactionIDIndex(transactionID, _coilSentTransactionIDs);
        if (index < 0) {    //  invalid if less than 0.
            assert(false);
            return;
        }
        beginAddress = index * B1ModbusProtocol::FUNCTION_CODE_READ_MAX_QTY_OF_COILS;
    }
    for (size_t i = 0; i < data.size(); ++i) {
        uint8 tmpData = data[i];
        if (tmpData == 0) {
            for (size_t j = 0; beginAddress < _coils.size() && j < 8; ++j) {
                _coils[beginAddress++] = 0;
            }
        }
        else {
            for (size_t j = 0; beginAddress < _coils.size() && j < 8; ++j) {
                _coils[beginAddress++] = tmpData % 2;
                tmpData = tmpData >> 1;
            }
        }
    }

#if (PRINT_UPDATED_VALUES)
    B1String msg;
    for (size_t i = 1; i < _coils.size(); ++i) {
        if (coils[i] != _coils[i]) {
            msg.appendf(" [%d:%d]", i, _coils[i] ? 1 : 0);
        }
    }
    if (msg.isEmpty() != true) {
        B1LOG("coil updated: %s", msg.cString());
    }
#endif
}

void B1ModbusManager::onFunctionWriteSingleCoil(uint16 address, uint16 value)
{
    //B1LOG("written coil: address[%d] value[%d]", address, value);
}

void B1ModbusManager::onFunctionWriteMultipleCoils(uint16 beginAddress, uint16 count)
{
    //B1LOG("written coils: address[%d] count[%d]", beginAddress, count);
}

void B1ModbusManager::onFunctionReadInputRegister(const std::vector<uint16>& data)
{
}

void B1ModbusManager::onFunctionReadHoldingRegisters(uint16 transactionID, const std::vector<uint16>& data)
{
#if (PRINT_UPDATED_VALUES)
    std::vector<uint16> holdingRegisters = _holdingRegisters;
#endif

    uint16 beginAddress = 0;
    if (_registerSentTransactionIDs.empty() != true) {
        int32 index = foundTransactionIDIndex(transactionID, _registerSentTransactionIDs);
        if (index < 0) {    //  invalid if less than 0.
            assert(false);
            return;
        }
        beginAddress = index * B1ModbusProtocol::FUNCTION_CODE_READ_MAX_QTY_OF_REGISTERS;
    }
    for (size_t i = 0; i < data.size(); ++i) {
        _holdingRegisters[beginAddress + i] = data[i];
    }

#if (PRINT_UPDATED_VALUES)
    B1String msg;
    for (size_t i = 0; i < _holdingRegisters.size(); ++i) {
        if (holdingRegisters[i] != _holdingRegisters[i]) {
            msg.appendf(" [%d:%d]", i, _holdingRegisters[i]);
        }
    }
    if (msg.isEmpty() != true) {
        B1LOG("holding register updated: %s", msg.cString());
    }
#endif
}

void B1ModbusManager::onFunctionWriteSingleRegister(uint16 address, uint16 value)
{
    //B1LOG("written register: address[%d] value[%d]", address, value);
}

void B1ModbusManager::onFunctionWriteMultipleRegisters(uint16 beginAddress, uint16 count)
{
    //B1LOG("written registers: address[%d] count[%d]", beginAddress, count);
}

bool B1ModbusManager::implInitialize(const B1String& address, uint16 port)
{
    if (_client != NULL) {
        return false;
    }
    _client.reset(new B1ModbusClient(this));
    if (_client->initialize(address, port) != true) {
        _client.reset();
        return false;
    }
    uint16 coilSetSize = static_cast<uint16>(_coils.size()) / B1ModbusProtocol::FUNCTION_CODE_READ_MAX_QTY_OF_COILS + 1;
    if (coilSetSize > 1) {
        _coilSentTransactionIDs.assign(coilSetSize, 0);
    }
    uint16 registerSetSize = static_cast<uint16>(_holdingRegisters.size()) / B1ModbusProtocol::FUNCTION_CODE_READ_MAX_QTY_OF_REGISTERS + 1;
    if (registerSetSize > 1) {
        _registerSentTransactionIDs.assign(registerSetSize, 0);
    }
    return true;
}

void B1ModbusManager::implFinalize()
{
    if (_client) {
        _client->finalize();
        _client.reset();
    }
    _sendData.clear();
    _coilSentTransactionIDs.clear();
    _registerSentTransactionIDs.clear();
}

bool B1ModbusManager::implProcess()
{
    std::vector<std::shared_ptr<SendData> > sendData;
    {
        B1AutoLock al(*_sendLock);
        sendData.swap(_sendData);
        _sendData.reserve(8);
    }
    for (auto s : sendData) {
        switch (s->_type) {
            break;
            case SEND_DATA_WRITE_SINGLE_COIL:
                sendWriteSingleCoil(*static_cast<SendDataBool*>(s.get()));
                break;
            case SEND_DATA_WRITE_MUILTIPLE_COILS:
                sendWriteMultipleCoils(*static_cast<SendDataBools*>(s.get()));
                break;
            case SEND_DATA_WRITE_SINGLE_REGISTER:
                sendWriteSingleRegister(*static_cast<SendDataValue*>(s.get()));
                break;
            case SEND_DATA_WRITE_MULTIPLE_REGISTERS:
                sendWriteMultipleRegisters(*static_cast<SendDataUint16s*>(s.get()));
                break;
            default:
                assert(false);
                return false;
        }
        B1Thread::sleep(CONSTS_SEND_DATA_INTERVAL);
    }
    return sendUpdateMemoryValue();
}

bool B1ModbusManager::requestWriteSingleCoil(uint16 address, bool set)
{
    B1AutoLock al(*_sendLock);
    _sendData.push_back(std::make_shared<SendDataBool>(SEND_DATA_WRITE_SINGLE_COIL, address, set));
    return true;
}

bool B1ModbusManager::requestWriteMultipleCoils(uint16 beginAddress, const std::vector<bool>& coils)
{
    B1AutoLock al(*_sendLock);
    _sendData.push_back(std::make_shared<SendDataBools>(SEND_DATA_WRITE_MUILTIPLE_COILS, beginAddress, coils));
    return true;
}

bool B1ModbusManager::requestWriteSingleRegister(uint16 address, uint16 value)
{
    B1AutoLock al(*_sendLock);
    _sendData.push_back(std::make_shared<SendDataValue>(SEND_DATA_WRITE_SINGLE_REGISTER, address, value));
    return true;
}

bool B1ModbusManager::requestWriteMultipleRegisters(uint16 beginAddress, const std::vector<uint16>& values)
{
    B1AutoLock al(*_sendLock);
    _sendData.push_back(std::make_shared<SendDataUint16s>(SEND_DATA_WRITE_MULTIPLE_REGISTERS, beginAddress, values));
    return true;
}
