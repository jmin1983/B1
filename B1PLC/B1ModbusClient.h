//
// B1ModbusClient.h
//
// Library: B1PLC
// Package: B1PLC
// Module:  B1PLC
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1PLC_MODBUS_CLIENT_H
#define _B1PLC_MODBUS_CLIENT_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <B1Network/B1BaseClient.h>

namespace BnD {
    class B1ModbusClientListener;
    class B1ModbusClientSession;
    class B1ModbusClient : protected B1BaseClient {
    public:
        B1ModbusClient(B1ModbusClientListener* listener);
    protected:
        B1ModbusClientListener* _listener;
    protected:
        B1BaseClientSession* createSession(B1ClientSocket* clientSocket, void* param) final;
    protected:
        std::shared_ptr<B1ModbusClientSession> getSession();
    public:
        bool initialize(const B1String& address, uint16 port);
        void finalize();
        bool sendReadDiscreteInputs(uint16 beginAddress, uint16 count);
        bool sendReadCoil(uint16 beginAddress, uint16 count, uint16* transactionID);
        bool sendWriteSingleCoil(uint16 address, uint16 value);
        bool sendWriteMultipleCoils(uint16 beginAddress, const std::vector<uint8>& values);
        bool sendReadInputRegister(uint16 beginAddress, uint16 count);
        bool sendReadHoldingRegisters(uint16 beginAddress, uint16 count, uint16* transactionID);
        bool sendWriteSingleRegister(uint16 address, uint16 value);
        bool sendWriteMultipleRegisters(uint16 beginAddress, const std::vector<uint16>& values);
    };
}   //  !BnD

#endif  // !_B1PLC_MODBUS_CLIENT_H
