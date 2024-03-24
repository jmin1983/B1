//
// B1ModbusClientSession.h
//
// Library: B1PLC
// Package: B1PLC
// Module:  B1PLC
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1PLC_MODBUS_CLIENT_SESSION_H
#define _B1PLC_MODBUS_CLIENT_SESSION_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif
 
#include <B1Network/B1ArrayBufferClientSession.h>
#include <B1PLC/B1ModbusClientPacketAnalyzer.h>

namespace BnD {
    class B1ModbusClientListener;
    class B1ModbusClientPacketMaker;
    class B1ModbusClientSession : protected B1ModbusClientPacketAnalyzer
                                , public B1ArrayBufferClientSession {
    public:
        B1ModbusClientSession(B1ClientSocket* clientSocket, B1BaseClientSessionListener* clientSessionListener, B1ModbusClientListener* modbusClientListener);
    protected:
        uint64 _lastReconnectTick;
        B1ModbusClientListener* _modbusClientListener;
        std::shared_ptr<B1ModbusClientPacketMaker> _packetMaker;
    protected:  //  B1ModbusPacketAnalyzer
        void onClientFunctionReadDiscreteInputs(const std::vector<uint8>& data) final;
        void onClientFunctionReadCoil(uint16 transactionID, const std::vector<uint8>& data) final;
        void onClientFunctionWriteSingleCoil(uint16 address, uint16 value) final;
        void onClientFunctionWriteMultipleCoils(uint16 beginAddress, uint16 count) final;
        void onClientFunctionReadInputRegister(const std::vector<uint16>& data) final;
        void onClientFunctionReadHoldingRegisters(uint16 transactionID, const std::vector<uint16>& data) final;
        void onClientFunctionWriteSingleRegister(uint16 address, uint16 value) final;
        void onClientFunctionWriteMultipleRegisters(uint16 beginAddress, uint16 count) final;
    protected:  //  B1ArrayBufferClientSession
        void onReadComplete(uint8* data, size_t dataSize) final;
        void implOnConnect() final;
        void implProcessDisconnected() final;
    public:
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

#endif  // !_B1PLC_MODBUS_CLIENT_SESSION_H
