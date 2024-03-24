//
// B1ModbusClientPacketMaker.h
//
// Library: B1PLC
// Package: B1PLC
// Module:  B1PLC
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1PLC_MODBUS_CLIENT_PACKET_MAKER_H
#define _B1PLC_MODBUS_CLIENT_PACKET_MAKER_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <B1PLC/B1ModbusPacketMaker.h>

namespace BnD {
    class B1ModbusClientPacketMaker : public B1ModbusPacketMaker {
    public:
        B1ModbusClientPacketMaker();
        virtual ~B1ModbusClientPacketMaker();
    private:
        uint16 _transactionID;
    public:
        void initialze() { _transactionID = 0; }
        std::vector<uint8> makeReadDiscreteInputs(uint16 beginAddress, uint16 count);
        std::vector<uint8> makeReadCoil(uint16 beginAddress, uint16 count, uint16* transactionID);
        std::vector<uint8> makeWriteSingleCoil(uint16 address, uint16 value);           //  value: 0 or 0xff
        std::vector<uint8> makeWriteMultipleCoils(uint16 beginAddress, const std::vector<uint8>& values);
        std::vector<uint8> makeReadInputRegister(uint16 beginAddress, uint16 count);
        std::vector<uint8> makeReadHoldingRegisters(uint16 beginAddress, uint16 count, uint16* transactionID);
        std::vector<uint8> makeWriteSingleRegister(uint16 address, uint16 value);
        std::vector<uint8> makeWriteMultipleRegisters(uint16 beginAddress, const std::vector<uint16>& values);
    };
}   //  !BnD

#endif  // !_B1PLC_MODBUS_CLIENT_PACKET_MAKER_H
