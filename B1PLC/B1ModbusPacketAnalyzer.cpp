//
// B1ModbusPacketAnalyzer.cpp
//
// Library: B1PLC
// Package: B1PLC
// Module:  B1PLC
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1PLC.h"
#include "B1ModbusPacketAnalyzer.h"
#include "B1ModbusProtocol.h"

#include <B1Base/B1Endian.h>

using namespace BnD;

B1ModbusPacketAnalyzer::B1ModbusPacketAnalyzer()
{
}

void B1ModbusPacketAnalyzer::onFunctionErrorIllegalFunction(uint16 transactionID, uint8 function, uint8 errorCode)
{
    B1LOG("not suppported function code(%d): transactionID[%d], function[%d]", errorCode, transactionID, function);
}

void B1ModbusPacketAnalyzer::onFunctionErrorIllegalDataAddress(uint16 transactionID, uint8 function, uint8 errorCode)
{
    B1LOG("requested data address is overflowed(%d): transactionID[%d], function[%d]", errorCode, transactionID, function);
}

void B1ModbusPacketAnalyzer::onFunctionErrorIllegalDataValue(uint16 transactionID, uint8 function, uint8 errorCode)
{
    B1LOG("requested data value is invalid(%d): transactionID[%d], function[%d]", errorCode, transactionID, function);
}

void B1ModbusPacketAnalyzer::onFunctionErrorOperationFailed(uint16 transactionID, uint8 function, uint8 errorCode)
{
    B1LOG("critical error occurred(%d): transactionID[%d], function[%d]", errorCode, transactionID, function);
}

auto B1ModbusPacketAnalyzer::implAnalyzeData(uint8* data, size_t size, size_t* pos) -> ANALYZE_RESULT
{
    B1ModbusProtocol::MBAP mbap;
    const size_t headerSize = sizeof(mbap);
    if (size < headerSize) {
        return ANALYZE_RESULT_NOT_ENOUTH_DATA;
    }
    memcpy(&mbap, data, headerSize);
    mbap._transactionID = TO_UINT16_FOR_NETWORK(mbap._transactionID);
    mbap._length = TO_UINT16_FOR_NETWORK(mbap._length);
    size_t remainSize = size - headerSize;
    if (remainSize + 1 < mbap._length) {
        return ANALYZE_RESULT_NOT_ENOUTH_DATA;
    }
    const size_t totalDataSize = headerSize - 1 + mbap._length;
    B1ModbusProtocol::PDU pdu;
    pdu._functionCode = *(data + headerSize);
    pdu._data.insert(pdu._data.end(), data + headerSize + 1, data + totalDataSize);
    (*pos) += totalDataSize;
    if (parseFunctionData(mbap._transactionID, pdu._functionCode, pdu._data) != true) {
        return ANALYZE_RESULT_FAIL;
    }
    return ANALYZE_RESULT_SUCCESS;
}

uint16 B1ModbusPacketAnalyzer::toUint16(const uint8* data) const
{
    uint16 value;
    memcpy(&value, data, 2);
    return TO_UINT16_FOR_NETWORK(value);
}

bool B1ModbusPacketAnalyzer::parseFunctionData(uint16 transactionID, uint8 function, const std::vector<uint8>& data)
{
    if (data.empty()) {
        return false;
    }
    switch (function) {
        case B1ModbusProtocol::FUNCTION_CODE_READ_DISCRETE_INPUTS:
            onFunctionReadDiscreteInputs(transactionID, data);
            break;
        case B1ModbusProtocol::FUNCTION_CODE_READ_COIL:
            onFunctionReadCoil(transactionID, data);
            break;
        case B1ModbusProtocol::FUNCTION_CODE_WRITE_SINGLE_COIL:
            onFunctionWriteSingleCoil(transactionID, data);
            break;
        case B1ModbusProtocol::FUNCTION_CODE_WRITE_MULTIPLE_COILS:
            onFunctionWriteMultipleCoils(transactionID, data);
            break;
        case B1ModbusProtocol::FUNCTION_CODE_READ_INPUT_REGISTER:
            onFunctionReadInputRegister(transactionID, data);
            break;
        case B1ModbusProtocol::FUNCTION_CODE_READ_HOLDING_REGISTERS:
            onFunctionReadHoldingRegisters(transactionID, data);
            break;
        case B1ModbusProtocol::FUNCTION_CODE_WRITE_SINGLE_REGISTER:
            onFunctionWriteSingleRegister(transactionID, data);
            break;
        case B1ModbusProtocol::FUNCTION_CODE_WRITE_MULTIPLE_REGISTERS:
            onFunctionWriteMultipleRegisters(transactionID, data);
            break;
        default:
            if (checkFunctionError(transactionID, function, data) != true) {
                B1LOG("unkown function code: function[%d][0x%x]", function, function);
                return false;
            }
            break;
    }
    return true;
}

bool B1ModbusPacketAnalyzer::checkFunctionError(uint16 transactionID, uint8 function, const std::vector<uint8>& data)
{
    if (function < B1ModbusProtocol::FUNCTION_CODE_ERROR_ADD_VALUE || data.empty()) {
        return false;
    }
    uint8 errorFunction = function - B1ModbusProtocol::FUNCTION_CODE_ERROR_ADD_VALUE;
    switch (errorFunction) {
        case B1ModbusProtocol::FUNCTION_CODE_READ_DISCRETE_INPUTS:
        case B1ModbusProtocol::FUNCTION_CODE_READ_COIL:
        case B1ModbusProtocol::FUNCTION_CODE_WRITE_SINGLE_COIL:
        case B1ModbusProtocol::FUNCTION_CODE_WRITE_MULTIPLE_COILS:
        case B1ModbusProtocol::FUNCTION_CODE_READ_INPUT_REGISTER:
        case B1ModbusProtocol::FUNCTION_CODE_READ_HOLDING_REGISTERS:
        case B1ModbusProtocol::FUNCTION_CODE_WRITE_SINGLE_REGISTER:
        case B1ModbusProtocol::FUNCTION_CODE_WRITE_MULTIPLE_REGISTERS:
            switch (data.front()) {
                case B1ModbusProtocol::FUNCTION_CODE_ERROR_ILLEGAL_FUNCTION:
                    onFunctionErrorIllegalFunction(transactionID, errorFunction, data.front());
                    break;
                case B1ModbusProtocol::FUNCTION_CODE_ERROR_ILLEGAL_DATA_ADDRESS:
                    onFunctionErrorIllegalDataAddress(transactionID, errorFunction, data.front());
                    break;
                case B1ModbusProtocol::FUNCTION_CODE_ERROR_ILLEGAL_DATA_VALUE:
                    onFunctionErrorIllegalDataValue(transactionID, errorFunction, data.front());
                    break;
                case B1ModbusProtocol::FUNCTION_CODE_ERROR_OPERATION_FAILED:
                    onFunctionErrorOperationFailed(transactionID, errorFunction, data.front());
                    break;
                default:
                    B1LOG("unkown function error code: function[%d], code[%d]", errorFunction, data.front());
                    return false;
            }
            break;
        default:
            return false;
    }
    return true;
}
