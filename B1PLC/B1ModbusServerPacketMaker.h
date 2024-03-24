//
// B1ModbusServerPacketMaker.h
//
// Library: B1PLC
// Package: B1PLC
// Module:  B1PLC
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1PLC_MODBUS_SERVER_PACKET_MAKER_H
#define _B1PLC_MODBUS_SERVER_PACKET_MAKER_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <B1PLC/B1ModbusPacketMaker.h>

namespace BnD {
    class B1ModbusServerPacketMaker : public B1ModbusPacketMaker {
    public:
        std::vector<uint8> makeReadDiscreteInputs(uint16 transactionID, uint16 beginAddress, uint16 count);
        std::vector<uint8> makeReadCoil(uint16 transactionID, const std::vector<bool>& values);
        std::vector<uint8> makeWriteSingleCoil(uint16 transactionID, uint16 address, uint16 value);
        std::vector<uint8> makeWriteMultipleCoils(uint16 transactionID, uint16 beginAddress, const std::vector<uint8>& values);
        std::vector<uint8> makeReadInputRegister(uint16 transactionID, uint16 beginAddress, uint16 count);
        std::vector<uint8> makeReadHoldingRegisters(uint16 transactionID, const std::vector<uint16>& values);
        std::vector<uint8> makeWriteSingleRegister(uint16 transactionID, uint16 address, uint16 value);
        std::vector<uint8> makeWriteMultipleRegisters(uint16 transactionID, uint16 beginAddress, const std::vector<uint16>& values);
        std::vector<uint8> makeExceptions(uint16 transactionID, uint8 code, uint8 exceptionCode);
    };
}   //  !BnD

#endif  // !_B1PLC_MODBUS_SERVER_PACKET_MAKER_H
