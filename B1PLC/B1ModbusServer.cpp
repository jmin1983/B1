//
// B1ModbusServer.cpp
//
// Library: B1PLC
// Package: B1PLC
// Module:  B1PLC
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1PLC.h"
#include "B1ModbusServer.h"
#include "B1ModbusServerSession.h"
#include "B1ModbusServerMemoryManager.h"
#include "B1ModbusProtocol.h"

using namespace BnD;

B1ModbusServer::B1ModbusServer()
    : B1BaseServer()
    , _memoryManager()
{
}

B1ModbusServer::~B1ModbusServer()
{
}

B1BaseServerSession* B1ModbusServer::createSession(B1ServerSocket* serverSocket)
{
    return new B1ModbusServerSession(serverSocket, this, _memoryManager.get());
}

bool B1ModbusServer::initialize(uint16 port, uint16 coilSize, uint16 holdingRegisterSize)
{
    if (port < 1 || port > UINT16_MAX) {
        return false;
    }
    if (startup() != true) {
        return false;
    }
    if (accept(static_cast<uint16>(port)) != true) {
        shutdown();
        return false;
    }   
    _memoryManager.reset(new B1ModbusServerMemoryManager(holdingRegisterSize, coilSize));
    return true;
}

void B1ModbusServer::finalize()
{
    close();
    shutdown();
}
