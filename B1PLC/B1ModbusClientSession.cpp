//
// B1ModbusClientSession.cpp
//
// Library: B1PLC
// Package: B1PLC
// Module:  B1PLC
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1PLC.h"
#include "B1ModbusClientSession.h"
#include "B1ModbusClientListener.h"
#include "B1ModbusClientPacketMaker.h"
#include "B1ModbusProtocol.h"

#include <B1Base/B1TickUtil.h>

using namespace BnD;

B1ModbusClientSession::B1ModbusClientSession(B1ClientSocket* clientSocket, B1BaseClientSessionListener* clientSessionListener, B1ModbusClientListener* modbusClientListener)
    : B1ModbusClientPacketAnalyzer()
    , B1ArrayBufferClientSession(clientSocket, clientSessionListener)
    , _lastReconnectTick(0)
    , _modbusClientListener(modbusClientListener)
    , _packetMaker(new B1ModbusClientPacketMaker())
{
}

void B1ModbusClientSession::onClientFunctionReadDiscreteInputs(const std::vector<uint8>& data)
{
    if (_modbusClientListener) {
        _modbusClientListener->onFunctionReadDiscreteInputs(data);
    }
}

void B1ModbusClientSession::onClientFunctionReadCoil(uint16 transactionID, const std::vector<uint8>& data)
{
    if (_modbusClientListener) {
        _modbusClientListener->onFunctionReadCoil(transactionID, data);
    }
}

void B1ModbusClientSession::onClientFunctionWriteSingleCoil(uint16 address, uint16 value)
{
    if (_modbusClientListener) {
        _modbusClientListener->onFunctionWriteSingleCoil(address, value);
    }
}

void B1ModbusClientSession::onClientFunctionWriteMultipleCoils(uint16 beginAddress, uint16 count)
{
    if (_modbusClientListener) {
        _modbusClientListener->onFunctionWriteMultipleCoils(beginAddress, count);
    }
}

void B1ModbusClientSession::onClientFunctionReadInputRegister(const std::vector<uint16>& data)
{
    if (_modbusClientListener) {
        _modbusClientListener->onFunctionReadInputRegister(data);
    }
}

void B1ModbusClientSession::onClientFunctionReadHoldingRegisters(uint16 transactionID, const std::vector<uint16>& data)
{
    if (_modbusClientListener) {
        _modbusClientListener->onFunctionReadHoldingRegisters(transactionID, data);
    }
}

void B1ModbusClientSession::onClientFunctionWriteSingleRegister(uint16 address, uint16 value)
{
    if (_modbusClientListener) {
        _modbusClientListener->onFunctionWriteSingleRegister(address, value);
    }
}

void B1ModbusClientSession::onClientFunctionWriteMultipleRegisters(uint16 beginAddress, uint16 count)
{
    if (_modbusClientListener) {
        _modbusClientListener->onFunctionWriteMultipleRegisters(beginAddress, count);
    }
}

void B1ModbusClientSession::onReadComplete(uint8* data, size_t dataSize)
{
    if (analyzeData(data, dataSize) != true) {
        disconnect();
    }
}

void B1ModbusClientSession::implOnConnect()
{
    B1LOG("session connected -> clear buffer");
    clearBuffer();
    _packetMaker->initialze();
    B1ArrayBufferClientSession::implOnConnect();
}

void B1ModbusClientSession::implProcessDisconnected()
{
    uint64 now = B1TickUtil::currentTick();
    if (B1TickUtil::diffTick(_lastReconnectTick, now) >= B1ModbusProtocol::CONSTS_CLIENT_RECONNECT_INTERVAL) {
        _lastReconnectTick = now;
        reconnect();
    }
}

bool B1ModbusClientSession::sendReadDiscreteInputs(uint16 beginAddress, uint16 count)
{
    auto data = _packetMaker->makeReadDiscreteInputs(beginAddress, count);
    return writeData(data);
}

bool B1ModbusClientSession::sendReadCoil(uint16 beginAddress, uint16 count, uint16* transactionID)
{
    auto data = _packetMaker->makeReadCoil(beginAddress, count, transactionID);
    return writeData(data);
}

bool B1ModbusClientSession::sendWriteSingleCoil(uint16 address, uint16 value)
{
    auto data = _packetMaker->makeWriteSingleCoil(address, value);
    return writeData(data);
}

bool B1ModbusClientSession::sendWriteMultipleCoils(uint16 beginAddress, const std::vector<uint8>& values)
{
    auto data = _packetMaker->makeWriteMultipleCoils(beginAddress, values);
    return writeData(data);
}

bool B1ModbusClientSession::sendReadInputRegister(uint16 beginAddress, uint16 count)
{
    auto data = _packetMaker->makeReadInputRegister(beginAddress, count);
    return writeData(data);
}

bool B1ModbusClientSession::sendReadHoldingRegisters(uint16 beginAddress, uint16 count, uint16* transactionID)
{
    auto data = _packetMaker->makeReadHoldingRegisters(beginAddress, count, transactionID);
    return writeData(data);
}

bool B1ModbusClientSession::sendWriteSingleRegister(uint16 address, uint16 value)
{
    auto data = _packetMaker->makeWriteSingleRegister(address, value);
    return writeData(data);
}

bool B1ModbusClientSession::sendWriteMultipleRegisters(uint16 beginAddress, const std::vector<uint16>& values)
{
    auto data = _packetMaker->makeWriteMultipleRegisters(beginAddress, values);
    return writeData(data);
}
