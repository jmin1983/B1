//
// B1ModbusManager.h
//
// Library: B1PLC
// Package: B1PLC
// Module:  B1PLC
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1PLC_MODBUS_MANAGER_H
#define _B1PLC_MODBUS_MANAGER_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <B1PLC/B1BasePLCManager.h>
#include <B1PLC/B1ModbusClientListener.h>

namespace BnD {
    class B1Lock;
    class B1ModbusClient;
    class B1ModbusManager : protected B1ModbusClientListener
                          , public B1BasePLCManager {
    public:
        B1ModbusManager();
        virtual ~B1ModbusManager();
    private:
        enum CONSTS {
            CONSTS_SEND_DATA_INTERVAL = 100,    //  modbus 지원 장비에 sleep 없이 데이터 요청하니까 제대로 동작 안됐다.
        };
        enum SEND_DATA {
            SEND_DATA_NONE = -1,
            SEND_DATA_RESERVED = 0,
            SEND_DATA_WRITE_SINGLE_COIL,
            SEND_DATA_WRITE_MUILTIPLE_COILS,
            SEND_DATA_WRITE_SINGLE_REGISTER,
            SEND_DATA_WRITE_MULTIPLE_REGISTERS,
        };
        struct SendData {
            SendData(SEND_DATA type, uint16 beginAddress) : _type(type), _beginAddress(beginAddress) {}
            const SEND_DATA _type;
            const uint16 _beginAddress;
        };
        struct SendDataValue : SendData {
            SendDataValue(SEND_DATA type, uint16 beginAddress, uint16 value) : SendData(type, beginAddress), _value(value) {}
            const uint16 _value;
        };
        struct SendDataBool : SendData {
            SendDataBool(SEND_DATA type, uint16 beginAddress, bool set) : SendData(type, beginAddress), _set(set) {}
            const bool _set;
        };
        struct SendDataBools : SendData {
            SendDataBools(SEND_DATA type, uint16 beginAddress, const std::vector<bool>& data) : SendData(type, beginAddress), _data(data) {}
            const std::vector<bool> _data;
        };
        struct SendDataUint16s : SendData {
            SendDataUint16s(SEND_DATA type, uint16 beginAddress, const std::vector<uint16>& data) : SendData(type, beginAddress), _data(data) {}
            const std::vector<uint16> _data;
        };
        std::shared_ptr<B1Lock> _sendLock;
        std::vector<std::shared_ptr<SendData> > _sendData;
        std::vector<uint16> _coilSentTransactionIDs;
        std::vector<uint16> _registerSentTransactionIDs;
    protected:
        std::shared_ptr<B1ModbusClient> _client;
    private:
        bool sendUpdateMemoryValue();
        bool sendReadCoil(uint16 beginAddress, uint16 count, uint16* transactionID);
        bool sendWriteSingleCoil(const SendDataBool& data);
        bool sendWriteMultipleCoils(const SendDataBools& data);
        bool sendReadHoldingRegisters(uint16 beginAddress, uint16 count, uint16* transactionID);
        bool sendWriteSingleRegister(const SendDataValue& data);
        bool sendWriteMultipleRegisters(const SendDataUint16s& data);
    protected:
        int32 foundTransactionIDIndex(uint16 transactionID, const std::vector<uint16>& transactionIDs); //  return index. invalid index if less than zero.
    protected:  //  B1ModbusClientListener
        void onFunctionReadDiscreteInputs(const std::vector<uint8>& data) final;
        void onFunctionReadCoil(uint16 transactionID, const std::vector<uint8>& data) final;
        void onFunctionWriteSingleCoil(uint16 address, uint16 value) final;
        void onFunctionWriteMultipleCoils(uint16 beginAddress, uint16 count) final;
        void onFunctionReadInputRegister(const std::vector<uint16>& data) final;
        void onFunctionReadHoldingRegisters(uint16 transactionID, const std::vector<uint16>& data) final;
        void onFunctionWriteSingleRegister(uint16 address, uint16 value) final;
        void onFunctionWriteMultipleRegisters(uint16 beginAddress, uint16 count) final;
    protected:  //  B1BasePLCManager
        bool implInitialize(const B1String& address, uint16 port) final;
        void implFinalize() final;
        bool implProcess() final;
    protected:
        bool requestWriteSingleCoil(uint16 address, bool set);
        bool requestWriteMultipleCoils(uint16 beginAddress, const std::vector<bool>& coils);
        bool requestWriteSingleRegister(uint16 address, uint16 value);
        bool requestWriteMultipleRegisters(uint16 beginAddress, const std::vector<uint16>& values);
    };
}   //  !BnD

#endif  // !_B1PLC_MODBUS_MANAGER_H
