//
// B1ModbusServerMemoryManager.h
//
// Library: B1PLC
// Package: B1PLC
// Module:  B1PLC
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1PLC_MODBUS_SERVER_MEMORY_MANAGER_H
#define _B1PLC_MODBUS_SERVER_MEMORY_MANAGER_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

namespace BnD {
    class B1ModbusServerMemoryManager {
    public:
        B1ModbusServerMemoryManager(uint16 holdingRegisterSize, uint16 coilSize);
        virtual ~B1ModbusServerMemoryManager();
    protected:
        std::vector<uint16> _holdingRegisters;
        std::vector<bool> _coils;
    public:
        uint8 setHoldingRegister(uint16 address, uint16 value);     //  return 0 if success. return error_code if fail.
        uint8 setHoldingRegisters(uint16 beginAddress, const std::vector<uint16>& values);  //  return 0 if success. return error_code if fail.
        uint8 getHoldingRegister(uint16 address, uint16* value);    //  return 0 if success. return error_code if fail.
        uint8 getHoldingRegisters(uint16 beginAddress, uint16 count, std::vector<uint16>* values);
        uint8 setCoil(uint16 address, bool value);                  //  return 0 if success. return error_code if fail.
        uint8 setCoils(uint16 beginAddress, const std::vector<uint8>& values);  //  return 0 if success. return error_code if fail.
        uint8 getCoil(uint16 address, bool* value);                 //  return 0 if success. return error_code if fail.
        uint8 getCoils(uint16 beginAddress, uint16 count, std::vector<bool>* values);
    };
}   //  !BnD

#endif  // !_B1PLC_MODBUS_SERVER_MEMORY_MANAGER_H
