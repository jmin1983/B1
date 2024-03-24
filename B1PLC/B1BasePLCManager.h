//
// B1BasePLCManager.h
//
// Library: B1PLC
// Package: B1PLC
// Module:  B1PLC
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1PLC_BASE_PLC_MANAGER_H
#define _B1PLC_BASE_PLC_MANAGER_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

namespace BnD {
    class B1BasePLCManager {
    public:
        B1BasePLCManager();
        virtual ~B1BasePLCManager();
    protected:
        std::vector<bool> _coils;               //  coil addresses to read.
        std::vector<uint16> _holdingRegisters;  //  holding register addresses to read.
    protected:
        virtual bool implInitialize(const B1String& plcAddress, uint16 plcPort) { return true; }
        virtual void implFinalize() {}
        virtual bool implProcess() { return true; }
        virtual bool implSetServoPower(int32 slaveID, bool onOff) { return false; }
        virtual bool implSetMoveLifting(int32 slaveID, int32 level) { return false; }
        virtual bool implSetMoveStop(int32 slaveID) { return false; }
        virtual bool implSetReadRFID(int32 slaveID, bool set) { return false; }
        virtual bool implSetReadRFIDDone(int32 slaveID, bool set) { return false; }
        virtual bool implSetWriteRFID(int32 slaveID, const B1String& value) { return false; }
        virtual bool implGetRFIDValue(int32 slaveID, B1String* value) { return false; }
        virtual bool implGetSensorInput(int32 slaveID, int32 channel) { return false; }
        virtual bool implIsPLCAlive() { return false; }
        virtual bool implIsPLCOffline() { return false; }
        virtual bool implIsPLCAlarm() { return false; }
        virtual bool implIsLoadComplete(int32 slaveID) { return false; }
        virtual bool implIsKPinDown(int32 slaveID) { return false; }
        virtual bool implIsMotorBusy(int32 slaveID) { return false; }
        virtual bool implIsMotorError(int32 slaveID) { return false; }
        virtual bool implIsReadRFIDComplete(int32 slaveID) { return false; }
        virtual bool implIsServoOn(int32 slaveID) { return false; }
        virtual bool implIsNLimit(int32 slaveID) { return false; }
        virtual bool implIsPLimit(int32 slaveID) { return false; }
        virtual bool implIsOutOfService(int32 slaveID) { return false; }
        virtual float implGetActualPosition(int32 slaveID) { return -1.f; }
    public:
        const std::vector<bool>& coils() const { return _coils; }
        const std::vector<uint16>& holdingRegisters() const { return _holdingRegisters; }
        bool initialize(const B1String& plcAddress, uint16 plcPort, uint16 readCoilSize, uint16 readHoldingRegisterSize);
        void finalize();
        bool process();
        bool isCoilSet(uint16 address) const;
        uint16 holdingRegisterValue(uint16 address) const;
        bool setServoPower(int32 slaveID, bool onOff);
        bool setMoveLifting(int32 slaveID, int32 level);
        bool setMoveStop(int32 slaveID);
        bool setReadRFID(int32 slaveID, bool set);
        bool setReadRFIDDone(int32 slaveID, bool set);
        bool setWriteRFID(int32 slaveID, const B1String& value);
        bool getRFIDValue(int32 slaveID, B1String* value);
        bool getSensorInput(int32 slaveID, int32 channel);
        bool isPLCAlive();
        bool isPLCOffline();
        bool isPLCAlarm();
        bool isLoadComplete(int32 slaveID);
        bool isKPinDown(int32 slaveID);
        bool isMotorBusy(int32 slaveID);
        bool isMotorError(int32 slaveID);
        bool isReadRFIDComplete(int32 slaveID);
        bool isServoOn(int32 slaveID);
        bool isNLimit(int32 slaveID);
        bool isPLimit(int32 slaveID);
        bool isOutOfService(int32 slaveID);
        float getActualPosition(int32 slaveID);
    };
}   //  !BnD

#endif  // !_B1PLC_BASE_PLC_MANAGER_H
