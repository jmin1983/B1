//
// B1ModbusPacketAnalyzer.h
//
// Library: B1PLC
// Package: B1PLC
// Module:  B1PLC
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1PLC_MODBUS_PACKET_ANALYZER_H
#define _B1PLC_MODBUS_PACKET_ANALYZER_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <B1Network/B1BasePacketAnalyzer.h>

namespace BnD {
    class B1ModbusPacketAnalyzer : public B1BasePacketAnalyzer {
    public:
        B1ModbusPacketAnalyzer();
    protected:
        virtual void onFunctionReadDiscreteInputs(uint16 transactionID, const std::vector<uint8>& data) {}
        virtual void onFunctionReadCoil(uint16 transactionID, const std::vector<uint8>& data) {}
        virtual void onFunctionWriteSingleCoil(uint16 transactionID, const std::vector<uint8>& data) {}
        virtual void onFunctionWriteMultipleCoils(uint16 transactionID, const std::vector<uint8>& data) {}
        virtual void onFunctionReadInputRegister(uint16 transactionID, const std::vector<uint8>& data) {}
        virtual void onFunctionReadHoldingRegisters(uint16 transactionID, const std::vector<uint8>& data) {}
        virtual void onFunctionWriteSingleRegister(uint16 transactionID, const std::vector<uint8>& data) {}
        virtual void onFunctionWriteMultipleRegisters(uint16 transactionID, const std::vector<uint8>& data) {}

        virtual void onFunctionErrorIllegalFunction(uint16 transactionID, uint8 function, uint8 errorCode);
        virtual void onFunctionErrorIllegalDataAddress(uint16 transactionID, uint8 function, uint8 errorCode);
        virtual void onFunctionErrorIllegalDataValue(uint16 transactionID, uint8 function, uint8 errorCode);
        virtual void onFunctionErrorOperationFailed(uint16 transactionID, uint8 function, uint8 errorCode);
    protected:
        ANALYZE_RESULT implAnalyzeData(uint8* data, size_t size, size_t* pos) final;
    protected:
        uint16 toUint16(const uint8* data) const;
        bool parseFunctionData(uint16 transactionID, uint8 function, const std::vector<uint8>& data);
        bool checkFunctionError(uint16 transactionID, uint8 function, const std::vector<uint8>& data);  //  return false if not an error code.
    };
}   //  !BnD

#endif  // !_B1PLC_MODBUS_PACKET_ANALYZER_H
