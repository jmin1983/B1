//
// B1ModbusServerSession.h
//
// Library: B1PLC
// Package: B1PLC
// Module:  B1PLC
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1PLC_MODBUS_SERVER_SESSION_H
#define _B1PLC_MODBUS_SERVER_SESSION_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <B1Network/B1ArrayBufferServerSession.h>
#include <B1PLC/B1ModbusServerMemoryManager.h>
#include <B1PLC/B1ModbusServerPacketAnalyzer.h>

namespace BnD {
    class B1ModbusServerMemoryManager;
    class B1ModbusServerPacketMaker;
    class B1ModbusServerSession : protected B1ModbusServerPacketAnalyzer
                                , public B1ArrayBufferServerSession {
    public:
        B1ModbusServerSession(B1ServerSocket* serverSocket, B1BaseServerSessionListener* listener, B1ModbusServerMemoryManager* memoryManager);
        virtual ~B1ModbusServerSession();
    protected:
        B1ModbusServerMemoryManager* _memeryManager;
        std::shared_ptr<B1ModbusServerPacketMaker> _packetMaker;
    protected:  //  B1ModbusServerPacketAnalyzer
        virtual void onServerFunctionReadDiscreteInputs(uint16 transactionID, uint16 beginAddress, uint16 count) override;
        virtual void onServerFunctionReadCoil(uint16 transactionID, uint16 beginAddress, uint16 count) override;
        virtual bool onServerFunctionWriteSingleCoil(uint16 transactionID, uint16 address, uint16 value) override;
        virtual bool onServerFunctionWriteMultipleCoils(uint16 transactionID, uint16 beginAddress, const std::vector<uint8>& values) override;
        virtual void onServerFunctionReadInputRegister(uint16 transactionID, uint16 beginAddress, uint16 count) override;
        virtual void onServerFunctionReadHoldingRegisters(uint16 transactionID, uint16 beginAddress, uint16 count) override;
        virtual bool onServerFunctionWriteSingleRegister(uint16 transactionID, uint16 address, uint16 value) override;
        virtual bool onServerFunctionWriteMultipleRegisters(uint16 transactionID, uint16 beginAddress, const std::vector<uint16>& values) override;
    protected:  //  B1ArrayBufferServerSession
        void onReadComplete(uint8* data, size_t dataSize) final;
    };
}   //  !BnD

#endif  // !_B1PLC_MODBUS_SERVER_SESSION_H
