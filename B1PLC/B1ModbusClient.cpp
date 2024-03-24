//
// B1ModbusClient.cpp
//
// Library: B1PLC
// Package: B1PLC
// Module:  B1PLC
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1PLC.h"
#include "B1ModbusClient.h"
#include "B1ModbusClientSession.h"

#include <B1Network/B1BaseSessionManager.h>

using namespace BnD;

B1ModbusClient::B1ModbusClient(B1ModbusClientListener* listener)
    : B1BaseClient()
    , _listener(listener)
{
}

B1BaseClientSession* B1ModbusClient::createSession(B1ClientSocket* clientSocket, void* param)
{
    return new B1ModbusClientSession(clientSocket, this, _listener);
}

std::shared_ptr<B1ModbusClientSession> B1ModbusClient::getSession()
{
    return sessionManager()->getUniqueSession<B1ModbusClientSession>();
}

bool B1ModbusClient::initialize(const B1String& address, uint16 port)
{
    if (startup() != true) {
        return false;
    }
    auto clientHandle = B1BaseClient::connect(address.copy(), port);
    if (0 == clientHandle) {
        B1LOG("unable to connect: address[%s], port[%d]", address.cString(), port);
        return false;
    }
    return true;
}

void B1ModbusClient::finalize()
{
    shutdown();
}

bool B1ModbusClient::sendReadDiscreteInputs(uint16 beginAddress, uint16 count)
{
    if (auto session = getSession()) {
        return session->sendReadDiscreteInputs(beginAddress, count);
    }
    return false;
}

bool B1ModbusClient::sendReadCoil(uint16 beginAddress, uint16 count, uint16* transactionID)
{
    if (auto session = getSession()) {
        return session->sendReadCoil(beginAddress, count, transactionID);
    }
    return false;
}

bool B1ModbusClient::sendWriteSingleCoil(uint16 address, uint16 value)
{
    if (auto session = getSession()) {
        return session->sendWriteSingleCoil(address, value);
    }
    return false;
}

bool B1ModbusClient::sendWriteMultipleCoils(uint16 beginAddress, const std::vector<uint8>& values)
{
    if (auto session = getSession()) {
        return session->sendWriteMultipleCoils(beginAddress, values);
    }
    return false;
}

bool B1ModbusClient::sendReadInputRegister(uint16 beginAddress, uint16 count)
{
    if (auto session = getSession()) {
        return session->sendReadInputRegister(beginAddress, count);
    }
    return false;
}

bool B1ModbusClient::sendReadHoldingRegisters(uint16 beginAddress, uint16 count, uint16* transactionID)
{
    if (auto session = getSession()) {
        return session->sendReadHoldingRegisters(beginAddress, count, transactionID);
    }
    return false;
}

bool B1ModbusClient::sendWriteSingleRegister(uint16 address, uint16 value)
{
    if (auto session = getSession()) {
        return session->sendWriteSingleRegister(address, value);
    }
    return false;
}

bool B1ModbusClient::sendWriteMultipleRegisters(uint16 beginAddress, const std::vector<uint16>& values)
{
    if (auto session = getSession()) {
        return session->sendWriteMultipleRegisters(beginAddress, values);
    }
    return false;
}
