//
// B1ModbusServerPacketAnalyzer.h
//
// Library: B1PLC
// Package: B1PLC
// Module:  B1PLC
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1PLC_MODBUS_SERVER_PACKET_ANALYZER_H
#define _B1PLC_MODBUS_SERVER_PACKET_ANALYZER_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <B1PLC/B1ModbusPacketAnalyzer.h>

namespace BnD {
    class B1ModbusServerPacketAnalyzer : public B1ModbusPacketAnalyzer {
    protected:
        virtual void onServerFunctionReadDiscreteInputs(uint16 transactionID, uint16 beginAddress, uint16 count) {}
        virtual void onServerFunctionReadCoil(uint16 transactionID, uint16 beginAddress, uint16 count) {}
        virtual bool onServerFunctionWriteSingleCoil(uint16 transactionID, uint16 address, uint16 value) { return false; }                                  //  return false if write fail.
        virtual bool onServerFunctionWriteMultipleCoils(uint16 transactionID, uint16 beginAddress, const std::vector<uint8>& values) { return false; }      //  return false if write fail.
        virtual void onServerFunctionReadInputRegister(uint16 transactionID, uint16 beginAddress, uint16 count) {}
        virtual void onServerFunctionReadHoldingRegisters(uint16 transactionID, uint16 beginAddress, uint16 count) {}
        virtual bool onServerFunctionWriteSingleRegister(uint16 transactionID, uint16 address, uint16 value) { return false; }                              //  return false if write fail.
        virtual bool onServerFunctionWriteMultipleRegisters(uint16 transactionID, uint16 beginAddress, const std::vector<uint16>& values) { return false; } //  return false if write fail.
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

#endif  // !_B1PLC_MODBUS_SERVER_PACKET_ANALYZER_H
