//
// B1ModbusProtocol.h
//
// Library: B1PLC
// Package: B1PLC
// Module:  B1PLC
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1PLC_MODBUS_PROTOCOL_H
#define _B1PLC_MODBUS_PROTOCOL_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

namespace BnD {
    namespace B1ModbusProtocol {
        enum CONSTS {
            CONSTS_CLIENT_RECONNECT_INTERVAL = 1000,
            CONSTS_MAX_ALIVE_CHECK_FAIL_COUNT = 3,
        };
        enum FUNCTION_CODE {
            FUNCTION_CODE_READ_DISCRETE_INPUTS =        2,
            FUNCTION_CODE_READ_COIL =                   1,
            FUNCTION_CODE_WRITE_SINGLE_COIL =           5,
            FUNCTION_CODE_WRITE_MULTIPLE_COILS =        15,
            FUNCTION_CODE_READ_INPUT_REGISTER =         4,
            FUNCTION_CODE_READ_HOLDING_REGISTERS =      3,
            FUNCTION_CODE_WRITE_SINGLE_REGISTER =       6,
            FUNCTION_CODE_WRITE_MULTIPLE_REGISTERS =    16,

            FUNCTION_CODE_ERROR_ADD_VALUE =             0x80,
            FUNCTION_CODE_ERROR_ILLEGAL_FUNCTION =      1,  //  not supported function code.
            FUNCTION_CODE_ERROR_ILLEGAL_DATA_ADDRESS =  2,  //  requested data address overflowed.
            FUNCTION_CODE_ERROR_ILLEGAL_DATA_VALUE =    3,  //  requested data quantity exceeded(참고: ILLEGAL_DATA_ADDRESS 보다 ILLEGAL_DATA_VALUE 의 처리 우선순위가 높다).
            FUNCTION_CODE_ERROR_OPERATION_FAILED =      4,  //  unrecoverable error occured.

            FUNCTION_CODE_READ_MAX_QTY_OF_COILS =       0x7D0,
            FUNCTION_CODE_READ_MAX_QTY_OF_REGISTERS =   0x7D,
            FUNCTION_CODE_WRITE_MAX_QTY_OF_COILS =      0x07B0,
            FUNCTION_CODE_WRITE_MAX_QTY_OF_REGISTERS =  0x007B,
        };
#pragma pack(1)
        struct MBAP {   //  ModbusApplicationProtocol.
            MBAP() : _transactionID(0), _protocolID(0), _length(0), _unitID(1) {}
            uint16 _transactionID;  //  for syncronization between messages of server and client.
            uint16 _protocolID;     //  0 for Modbus/TCP.
            uint16 _length;         //  number of remaining bytes in this frame.
            uint8 _unitID;          //  slave address(255 if not used).
        };
        struct PDU {    //  ProtocolDataUnit.
            uint8 _functionCode;    //  function codes as in other variants.
            std::vector<uint8> _data;
        };
        struct Frame {
            MBAP _header;
            PDU _pdu;
        };
#pragma pack()
    };
}   //  !BnD

#endif  // !_B1PLC_MODBUS_PROTOCOL_H
