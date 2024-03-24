//
// B1ModbusClientListener.h
//
// Library: B1PLC
// Package: B1PLC
// Module:  B1PLC
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1PLC_MODBUS_CLIENT_LISTENER_H
#define _B1PLC_MODBUS_CLIENT_LISTENER_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

namespace BnD {
    class B1ModbusClientListener {
    public:
        virtual void onFunctionReadDiscreteInputs(const std::vector<uint8>& data) {}
        virtual void onFunctionReadCoil(uint16 transactionID, const std::vector<uint8>& data) {}
        virtual void onFunctionWriteSingleCoil(uint16 address, uint16 value) {}
        virtual void onFunctionWriteMultipleCoils(uint16 beginAddress, uint16 count) {}
        virtual void onFunctionReadInputRegister(const std::vector<uint16>& data) {}
        virtual void onFunctionReadHoldingRegisters(uint16 transactionID, const std::vector<uint16>& data) {}
        virtual void onFunctionWriteSingleRegister(uint16 address, uint16 value) {}
        virtual void onFunctionWriteMultipleRegisters(uint16 beginAddress, uint16 count) {}
    };
}   //  !BnD

#endif  // !_B1PLC_MODBUS_CLIENT_LISTENER_H
