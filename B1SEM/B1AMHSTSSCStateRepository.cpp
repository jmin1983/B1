//
// B1AMHSTSSCStateRepository.cpp
//
// Library: B1SEM
// Package: B1SEM
// Module:  B1SEM
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1SEM.h"
#include "B1AMHSTSSCStateRepository.h"
#include "B1AMHSStates.h"
#include "B1AMHSTSSCStateRepositoryListener.h"

#include <B1Base/B1Lock.h>

using namespace BnD;
using namespace B1AMHSSEM;

B1AMHSTSSCStateRepository::B1AMHSTSSCStateRepository()
    : _listener(NULL)
    , _lock(new B1Lock())
    , _systemState(SYSTEM_STATE::NONE)
    , _systemAvailState(SYSTEM_AVAIL_STATE::NONE)
{
}

B1AMHSTSSCStateRepository::~B1AMHSTSSCStateRepository()
{
}

bool B1AMHSTSSCStateRepository::initialize(B1AMHSTSSCStateRepositoryListener* listener)
{
    _listener = listener;
    return true;
}

void B1AMHSTSSCStateRepository::finalize()
{
    _listener = NULL;
    _systemState = SYSTEM_STATE::NONE;
    _systemAvailState = SYSTEM_AVAIL_STATE::NONE;
}

void B1AMHSTSSCStateRepository::setSystemState(SYSTEM_STATE newState)
{
    SYSTEM_STATE prevState;
    {
        B1AutoLock al(*_lock);
        if (_systemState == newState) {
            return;
        }
        prevState = _systemState;
        _systemState = newState;
    }
    if (SYSTEM_STATE::NONE == prevState && SYSTEM_STATE::TSSC_INIT == newState) {
        if (_listener) {
            _listener->onActionTSSCAutoInitiated(prevState, newState);
        }
    }
    else if (SYSTEM_STATE::TSSC_INIT == prevState && SYSTEM_STATE::PAUSED == newState) {
        if (_listener) {
            _listener->onActionTSSCPaused(prevState, newState);
        }
    }
    else if (SYSTEM_STATE::PAUSED == prevState && SYSTEM_STATE::AUTO == newState) {
        if (_listener) {
            _listener->onActionTSSCAutoCompleted(prevState, newState);
        }
    }
    else if (SYSTEM_STATE::AUTO == prevState && SYSTEM_STATE::PAUSING == newState) {
        if (_listener) {
            _listener->onActionTSSCPauseInitiated(prevState, newState);
        }
    }
    else if (SYSTEM_STATE::PAUSING == prevState && SYSTEM_STATE::AUTO == newState) {
        if (_listener) {
            _listener->onActionTSSCAutoCompleted(prevState, newState);
        }
    }
    else if (SYSTEM_STATE::PAUSING == prevState && SYSTEM_STATE::PAUSED == newState) {
        if (_listener) {
            _listener->onActionTSSCPauseCompleted(prevState, newState);
        }
    }
    else {
        if (_listener) {
            _listener->onActionTSSCETC(prevState, newState);
        }
    }
}

void B1AMHSTSSCStateRepository::setSystemStatePausing()
{
    setSystemState(SYSTEM_STATE::PAUSING);
}

void B1AMHSTSSCStateRepository::setSystemStatePaused()
{
    setSystemState(SYSTEM_STATE::PAUSED);
}

void B1AMHSTSSCStateRepository::setSystemStateAuto()
{
    setSystemState(SYSTEM_STATE::AUTO);
}

void B1AMHSTSSCStateRepository::setSystemAvailState(SYSTEM_AVAIL_STATE newState)
{
    SYSTEM_AVAIL_STATE prevState;
    {
        B1AutoLock al(*_lock);
        if (_systemAvailState == newState) {
            return;
        }
        prevState = _systemAvailState;
        _systemAvailState = newState;
    }
    if (SYSTEM_AVAIL_STATE::IN_SERVICE == newState) {
        if (_listener) _listener->onActionTSSCInService(prevState, newState);
    }
    else if (SYSTEM_AVAIL_STATE::OUT_OF_SERVICE == newState) {
        if (_listener) _listener->onActionTSSCOutOfService(prevState, newState);
    }
}

void B1AMHSTSSCStateRepository::setSystemAvailStateInService()
{
    setSystemAvailState(SYSTEM_AVAIL_STATE::IN_SERVICE);
}

void B1AMHSTSSCStateRepository::setSystemAvailStateOutOfService()
{
    setSystemAvailState(SYSTEM_AVAIL_STATE::OUT_OF_SERVICE);
}

bool B1AMHSTSSCStateRepository::isSystemState(SYSTEM_STATE state) const
{
    B1AutoLock al(*_lock);
    return state == _systemState;
}

bool B1AMHSTSSCStateRepository::isSystemStatePausing() const
{
    return isSystemState(SYSTEM_STATE::PAUSING);
}

bool B1AMHSTSSCStateRepository::isSystemStatePaused() const
{
    return isSystemState(SYSTEM_STATE::PAUSED);
}

bool B1AMHSTSSCStateRepository::isSystemStateAuto() const
{
    return isSystemState(SYSTEM_STATE::AUTO);
}

bool B1AMHSTSSCStateRepository::isSystemAvailState(SYSTEM_AVAIL_STATE state) const
{
    B1AutoLock al(*_lock);
    return state == _systemAvailState;
}

bool B1AMHSTSSCStateRepository::isSystemAvailStateInService() const
{
    return isSystemAvailState(SYSTEM_AVAIL_STATE::IN_SERVICE);
}

bool B1AMHSTSSCStateRepository::isSystemAvailStateOutOfService() const
{
    return isSystemAvailState(SYSTEM_AVAIL_STATE::OUT_OF_SERVICE);
}

SYSTEM_STATE B1AMHSTSSCStateRepository::systemState() const
{
    B1AutoLock al(*_lock);
    return _systemState;
}

SYSTEM_AVAIL_STATE B1AMHSTSSCStateRepository::systemAvailState() const
{
    B1AutoLock al(*_lock);
    return _systemAvailState;
}
