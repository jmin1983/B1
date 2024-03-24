//
// B1ModbusClientPacketAnalyzer.h
//
// Library: B1PLC
// Package: B1PLC
// Module:  B1PLC
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1PLC_MODBUS_CLIENT_PACKET_ANALYZER_H
#define _B1PLC_MODBUS_CLIENT_PACKET_ANALYZER_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <B1PLC/B1ModbusPacketAnalyzer.h>

namespace BnD {
    class B1ModbusClientPacketAnalyzer : public B1ModbusPacketAnalyzer {
    protected:
        virtual void onClientFunctionReadDiscreteInputs(const std::vector<uint8>& data) {}
        virtual void onClientFunctionReadCoil(uint16 transactionID, const std::vector<uint8>& data) {}
        virtual void onClientFunctionWriteSingleCoil(uint16 address, uint16 value) {}
        virtual void onClientFunctionWriteMultipleCoils(uint16 beginAddress, uint16 count) {}
        virtual void onClientFunctionReadInputRegister(const std::vector<uint16>& data) {}
        virtual void onClientFunctionReadHoldingRegisters(uint16 transactionID, const std::vector<uint16>& data) {}
        virtual void onClientFunctionWriteSingleRegister(uint16 address, uint16 value) {}
        virtual void onClientFunctionWriteMultipleRegisters(uint16 beginAddress, uint16 count) {}
    protected:  //  B1ModbusPacketAnalyzer
        void onFunctionReadDiscreteInputs(uint16 transactionID, const std::vector<uint8>& data) final;
        void onFunctionReadCoil(uint16 transactionID, const std::vector<uint8>& data) final;
        void onFunctionWriteSingleCoil(uint16 transactionID, const std::vector<uint8>& data) final;
        void onFunctionWriteMultipleCoils(uint16 transactionID, const std::vector<uint8>& data) final;
        void onFunctionReadInputRegister(uint16 transactionID, const std::vector<uint8>& data) final;
        void onFunctionReadHoldingRegisters(uint16 transactionID, const std::vector<uint8>& data) final;
        void onFunctionWriteSingleRegister(uint16 transactionID, const std::vector<uint8>& data) final;
        void onFunctionWriteMultipleRegisters(uint16 transactionID, const std::vector<uint8>& data) final;
    };
}   //  !BnD

#endif  // !_B1PLC_MODBUS_CLIENT_PACKET_ANALYZER_H
