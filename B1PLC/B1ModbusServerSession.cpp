//
// B1ModbusServerSession.cpp
//
// Library: B1PLC
// Package: B1PLC
// Module:  B1PLC
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1PLC.h"
#include "B1ModbusServerSession.h"
#include "B1ModbusServerPacketMaker.h"
#include "B1ModbusServer.h"
#include "B1ModbusProtocol.h"

using namespace BnD;

B1ModbusServerSession::B1ModbusServerSession(B1ServerSocket* serverSocket, B1BaseServerSessionListener* listener, B1ModbusServerMemoryManager* memoryManager)
    : B1ModbusServerPacketAnalyzer()
    , B1ArrayBufferServerSession(serverSocket, listener)
    , _memeryManager(memoryManager)
    , _packetMaker(new B1ModbusServerPacketMaker())
{
    assert(_memeryManager != NULL);
}

B1ModbusServerSession::~B1ModbusServerSession()
{
}

void B1ModbusServerSession::onServerFunctionReadDiscreteInputs(uint16 transactionID, uint16 beginAddress, uint16 count)
{
    assert(false);  //  not implemented.
    auto data = _packetMaker->makeExceptions(transactionID, B1ModbusProtocol::FUNCTION_CODE_READ_DISCRETE_INPUTS, B1ModbusProtocol::FUNCTION_CODE_ERROR_OPERATION_FAILED);
    writeData(data);
}

void B1ModbusServerSession::onServerFunctionReadCoil(uint16 transactionID, uint16 beginAddress, uint16 count)
{
    std::vector<bool> values;
    auto result = _memeryManager->getCoils(beginAddress, count, &values);
    if (0 == result) {
        auto data = _packetMaker->makeReadCoil(transactionID, values);
        writeData(data);
    }
    else {
        auto data = _packetMaker->makeExceptions(transactionID, B1ModbusProtocol::FUNCTION_CODE_READ_COIL, result);
        writeData(data);
    }
}

bool B1ModbusServerSession::onServerFunctionWriteSingleCoil(uint16 transactionID, uint16 address, uint16 value)
{
    auto result = _memeryManager->setCoil(address, value);
    if (result != 0) {
        auto data = _packetMaker->makeExceptions(transactionID, B1ModbusProtocol::FUNCTION_CODE_WRITE_SINGLE_COIL, result);
        writeData(data);
        return false;
    }
    return true;
}

bool B1ModbusServerSession::onServerFunctionWriteMultipleCoils(uint16 transactionID, uint16 beginAddress, const std::vector<uint8>& values)
{
    auto result = _memeryManager->setCoils(beginAddress, values);
    if (result != 0) {
        auto data = _packetMaker->makeExceptions(transactionID, B1ModbusProtocol::FUNCTION_CODE_WRITE_MULTIPLE_COILS, result);
        writeData(data);
        return false;
    }
    return true;
}

void B1ModbusServerSession::onServerFunctionReadInputRegister(uint16 transactionID, uint16 beginAddress, uint16 count)
{
    assert(false);  //  not implemented.
    auto data = _packetMaker->makeExceptions(transactionID, B1ModbusProtocol::FUNCTION_CODE_READ_INPUT_REGISTER, B1ModbusProtocol::FUNCTION_CODE_ERROR_OPERATION_FAILED);
    writeData(data);
}

void B1ModbusServerSession::onServerFunctionReadHoldingRegisters(uint16 transactionID, uint16 beginAddress, uint16 count)
{
    std::vector<uint16> values;
    auto result = _memeryManager->getHoldingRegisters(beginAddress, count, &values);
    if (0 == result) {
        auto data = _packetMaker->makeReadHoldingRegisters(transactionID, values);
        writeData(data);
    }
    else {
        auto data = _packetMaker->makeExceptions(transactionID, B1ModbusProtocol::FUNCTION_CODE_READ_HOLDING_REGISTERS, result);
        writeData(data);
    }
}

bool B1ModbusServerSession::onServerFunctionWriteSingleRegister(uint16 transactionID, uint16 address, uint16 value)
{
    auto result = _memeryManager->setHoldingRegister(address, value);
    if (result != 0) {
        auto data = _packetMaker->makeExceptions(transactionID, B1ModbusProtocol::FUNCTION_CODE_WRITE_SINGLE_REGISTER, result);
        writeData(data);
        return false;
    }
    return true;
}

bool B1ModbusServerSession::onServerFunctionWriteMultipleRegisters(uint16 transactionID, uint16 beginAddress, const std::vector<uint16>& values)
{
    auto result = _memeryManager->setHoldingRegisters(beginAddress, values);
    if (result != 0) {
        auto data = _packetMaker->makeExceptions(transactionID, B1ModbusProtocol::FUNCTION_CODE_WRITE_MULTIPLE_REGISTERS, result);
        writeData(data);
        return false;
    }
    return true;
}

void B1ModbusServerSession::onReadComplete(uint8* data, size_t dataSize)
{
    if (analyzeData(data, dataSize) != true) {
        disconnect();
    }
}
