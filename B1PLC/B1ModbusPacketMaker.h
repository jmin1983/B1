//
// B1ModbusPacketMaker.h
//
// Library: B1PLC
// Package: B1PLC
// Module:  B1PLC
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1PLC_MODBUS_PACKET_MAKER_H
#define _B1PLC_MODBUS_PACKET_MAKER_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

namespace BnD {
    class B1ModbusPacketMaker {
    public:
        B1ModbusPacketMaker();
        virtual ~B1ModbusPacketMaker();
    protected:
        void insertUint16(uint16 value, std::vector<uint8>* buffer);
        void insertMBAP(uint16 transactionID, size_t dataSize, std::vector<uint8>* buffer);
        std::vector<uint8> makeFunctionData(uint16 transactionID, uint8 code, const std::vector<uint8>& data);
    };
}   //  !BnD

#endif  // !_B1PLC_MODBUS_PACKET_MAKER_H
