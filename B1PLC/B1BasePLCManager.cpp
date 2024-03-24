//
// B1BasePLCManager.cpp
//
// Library: B1PLC
// Package: B1PLC
// Module:  B1PLC
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1PLC.h"
#include "B1BasePLCManager.h"

using namespace BnD;

B1BasePLCManager::B1BasePLCManager()
    : _coils()
    , _holdingRegisters()
{
}

B1BasePLCManager::~B1BasePLCManager()
{
}

bool B1BasePLCManager::initialize(const B1String& plcAddress, uint16 plcPort, uint16 readCoilSize, uint16 readHoldingRegisterSize)
{
    _coils.resize(readCoilSize, false);
    _holdingRegisters.resize(readHoldingRegisterSize, 0);
    return implInitialize(plcAddress, plcPort);
}

void B1BasePLCManager::finalize()
{
    implFinalize();
}

bool B1BasePLCManager::process()
{
    return implProcess();
}

bool B1BasePLCManager::isCoilSet(uint16 address) const
{
    return _coils.size() > address ? _coils[address] : false;
}

uint16 B1BasePLCManager::holdingRegisterValue(uint16 address) const
{
    return _holdingRegisters.size() > address ? _holdingRegisters[address] : 0;
}

bool B1BasePLCManager::setServoPower(int32 slaveID, bool onOff)
{
    if (slaveID < 0) {
        assert(false);
        return false;
    }
    return implSetServoPower(slaveID, onOff);
}

bool B1BasePLCManager::setMoveLifting(int32 slaveID, int32 level)
{
    if (slaveID < 0) {
        assert(false);
        return false;
    }
    return implSetMoveLifting(slaveID, level);
}

bool B1BasePLCManager::setMoveStop(int32 slaveID)
{
    if (slaveID < 0) {
        assert(false);
        return false;
    }
    return implSetMoveStop(slaveID);
}

bool B1BasePLCManager::setReadRFID(int32 slaveID, bool set)
{
    if (slaveID < 0) {
        assert(false);
        return false;
    }
    return implSetReadRFID(slaveID, set);
}

bool B1BasePLCManager::setReadRFIDDone(int32 slaveID, bool set)
{
    if (slaveID < 0) {
        assert(false);
        return false;
    }
    return implSetReadRFIDDone(slaveID, set);
}

bool B1BasePLCManager::setWriteRFID(int32 slaveID, const B1String& value)
{
    if (slaveID < 0) {
        assert(false);
        return false;
    }
    return implSetWriteRFID(slaveID, value);
}

bool B1BasePLCManager::getRFIDValue(int32 slaveID, B1String* value)
{
    if (slaveID < 0) {
        assert(false);
        return false;
    }
    return implGetRFIDValue(slaveID, value);
}

bool B1BasePLCManager::getSensorInput(int32 slaveID, int32 channel)
{
    if (slaveID < 0) {
        assert(false);
        return false;
    }
    return implGetSensorInput(slaveID, channel);
}

bool B1BasePLCManager::isPLCAlive()
{
    return implIsPLCAlive();
}

bool B1BasePLCManager::isPLCOffline()
{
    return implIsPLCOffline();
}

bool B1BasePLCManager::isPLCAlarm()
{
    return implIsPLCAlarm();
}

bool B1BasePLCManager::isLoadComplete(int32 slaveID)
{
    if (slaveID < 0) {
        assert(false);
        return false;
    }
    return implIsLoadComplete(slaveID);
}

bool B1BasePLCManager::isKPinDown(int32 slaveID)
{
    if (slaveID < 0) {
        assert(false);
        return false;
    }
    return implIsKPinDown(slaveID);
}

bool B1BasePLCManager::isMotorBusy(int32 slaveID)
{
    if (slaveID < 0) {
        assert(false);
        return false;
    }
    return implIsMotorBusy(slaveID);
}

bool B1BasePLCManager::isMotorError(int32 slaveID)
{
    if (slaveID < 0) {
        assert(false);
        return false;
    }
    return implIsMotorError(slaveID);
}

bool B1BasePLCManager::isReadRFIDComplete(int32 slaveID)
{
    if (slaveID < 0) {
        assert(false);
        return false;
    }
    return implIsReadRFIDComplete(slaveID);
}

bool B1BasePLCManager::isServoOn(int32 slaveID)
{
    if (slaveID < 0) {
        assert(false);
        return false;
    }
    return implIsServoOn(slaveID);
}

bool B1BasePLCManager::isNLimit(int32 slaveID)
{
    if (slaveID < 0) {
        assert(false);
        return false;
    }
    return implIsNLimit(slaveID);
}

bool B1BasePLCManager::isPLimit(int32 slaveID)
{
    if (slaveID < 0) {
        assert(false);
        return false;
    }
    return implIsPLimit(slaveID);
}

bool B1BasePLCManager::isOutOfService(int32 slaveID)
{
    if (slaveID < 0) {
        assert(false);
        return false;
    }
    return implIsOutOfService(slaveID);
}

float B1BasePLCManager::getActualPosition(int32 slaveID)
{
    if (slaveID < 0) {
        assert(false);
        return 0;
    }
    return implGetActualPosition(slaveID);
}

