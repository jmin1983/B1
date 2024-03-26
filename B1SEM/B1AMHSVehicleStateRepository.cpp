//
// B1AMHSVehicleStateRepository.cpp
//
// Library: B1SEM
// Package: B1SEM
// Module:  B1SEM
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1SEM.h"
#include "B1AMHSVehicleStateRepository.h"
#include "B1AMHSVehicleStateRepositoryListener.h"
#include "B1AMHSStates.h"

#include <B1Base/B1Lock.h>

using namespace BnD;
using namespace B1AMHSSEM;

B1AMHSVehicleStateRepository::B1AMHSVehicleStateRepository()
    : _listener(NULL)
    , _lock(new B1Lock())
    , _states()
{
}

B1AMHSVehicleStateRepository::~B1AMHSVehicleStateRepository()
{
}

bool B1AMHSVehicleStateRepository::setState(int32 id, VEHICLE_ACT_STATE newState, VEHICLE_ACT_STATE* prevState)
{
    B1AutoLock al(*_lock);
    auto itr = _states.find(id);
    if (itr == _states.end()) {
        return false;
    }
    if (itr->second._activityState == newState) {
        return false;
    }
    *prevState = itr->second._activityState;
    itr->second._activityState = newState;
    return true;
}

bool B1AMHSVehicleStateRepository::setState(int32 id, VEHICLE_AVAIL_STATE newState, VEHICLE_AVAIL_STATE* prevState)
{
    B1AutoLock al(*_lock);
    auto itr = _states.find(id);
    if (itr == _states.end()) {
        return false;
    }
    if (itr->second._availState == newState) {
        return false;
    }
    *prevState = itr->second._availState;
    itr->second._availState = newState;
    return true;
}

bool B1AMHSVehicleStateRepository::isStateAssigned(VEHICLE_ACT_STATE state) const
{
    return VEHICLE_ACT_STATE::INSTALLED_ASSIGNED_ENROUTE == state ||
           VEHICLE_ACT_STATE::INSTALLED_ASSIGNED_PARKED == state ||
           VEHICLE_ACT_STATE::INSTALLED_ASSIGNED_ACQUIRING == state ||
           VEHICLE_ACT_STATE::INSTALLED_ASSIGNED_DEPOSITING == state;
}

bool B1AMHSVehicleStateRepository::isStateInstalled(VEHICLE_ACT_STATE state) const
{
    return VEHICLE_ACT_STATE::INSTALLED_NOT_ASSIGNED == state ||
           VEHICLE_ACT_STATE::INSTALLED_ASSIGNED_ENROUTE == state ||
           VEHICLE_ACT_STATE::INSTALLED_ASSIGNED_PARKED == state ||
           VEHICLE_ACT_STATE::INSTALLED_ASSIGNED_ACQUIRING == state ||
           VEHICLE_ACT_STATE::INSTALLED_ASSIGNED_DEPOSITING == state;
}

bool B1AMHSVehicleStateRepository::initialize(const std::map<int32, std::pair<B1AMHSSEM::VEHICLE_ACT_STATE, B1AMHSSEM::VEHICLE_AVAIL_STATE> >& states,
                                            B1AMHSVehicleStateRepositoryListener* listener)
{
    _listener = listener;
    for (const auto& state : states) {
        _states.insert(std::make_pair(state.first, state.second));
    }
    return true;
}

void B1AMHSVehicleStateRepository::finalize()
{
    _listener = NULL;
    _states.clear();
}

void B1AMHSVehicleStateRepository::setVehicleActivityState(int32 id, VEHICLE_ACT_STATE newState)
{
    VEHICLE_ACT_STATE prevState;
    if (setState(id, newState, &prevState) != true) {
        return;
    }
    if (VEHICLE_ACT_STATE::INSTALLED_ASSIGNED_ENROUTE == prevState && VEHICLE_ACT_STATE::INSTALLED_ASSIGNED_PARKED == newState) {
        if (_listener) {
            _listener->onActionVehicleArrived(id, prevState, newState);
        }
    }
    else if (VEHICLE_ACT_STATE::INSTALLED_ASSIGNED_PARKED == prevState && VEHICLE_ACT_STATE::INSTALLED_ASSIGNED_ENROUTE == newState) {
        if (_listener) {
            _listener->onActionVehicleDeparted(id, prevState, newState);
        }
    }
    else if (VEHICLE_ACT_STATE::INSTALLED_ASSIGNED_PARKED == prevState && VEHICLE_ACT_STATE::INSTALLED_ASSIGNED_ACQUIRING == newState) {
        if (_listener) {
            _listener->onActionVehicleAcquireStarted(id, prevState, newState);
        }
    }
    else if (VEHICLE_ACT_STATE::INSTALLED_ASSIGNED_ACQUIRING == prevState && VEHICLE_ACT_STATE::INSTALLED_ASSIGNED_PARKED == newState) {
        if (_listener) {
            _listener->onActionVehicleAcquireCompleted(id, prevState, newState);
        }
    }
    else if (VEHICLE_ACT_STATE::INSTALLED_ASSIGNED_PARKED == prevState && VEHICLE_ACT_STATE::INSTALLED_ASSIGNED_DEPOSITING == newState) {
        if (_listener) {
            _listener->onActionVehicleDepositStarted(id, prevState, newState);
        }
    }
    else if (VEHICLE_ACT_STATE::INSTALLED_ASSIGNED_DEPOSITING == prevState && VEHICLE_ACT_STATE::INSTALLED_ASSIGNED_PARKED == newState) {
        if (_listener) {
            _listener->onActionVehicleDepositCompleted(id, prevState, newState);
        }
    }
    else if (VEHICLE_ACT_STATE::INSTALLED_ASSIGNED_ACQUIRING == prevState && VEHICLE_ACT_STATE::INSTALLED_ASSIGNED_DEPOSITING == newState) {
        if (_listener) {
            _listener->onActionVehicleDepositStarted(id, prevState, newState);
        }
    }
    else if (VEHICLE_ACT_STATE::INSTALLED_ASSIGNED_DEPOSITING == prevState && VEHICLE_ACT_STATE::INSTALLED_ASSIGNED_ACQUIRING == newState) {
        if (_listener) {
            _listener->onActionVehicleAcquireStarted(id, prevState, newState);
        }
    }
    else if (isStateAssigned(prevState) && VEHICLE_ACT_STATE::INSTALLED_NOT_ASSIGNED == newState) {
        if (_listener) {
            _listener->onActionVehicleUnassigned(id, prevState, newState);
        }
    }
    else if (VEHICLE_ACT_STATE::INSTALLED_NOT_ASSIGNED == prevState && isStateAssigned(newState)) {
        if (_listener) {
            _listener->onActionVehicleAssigned(id, prevState, newState);
        }
    }
    else if (isStateInstalled(prevState) && VEHICLE_ACT_STATE::REMOVED == newState) {
        if (_listener) {
            _listener->onActionVehicleRemoved(id, prevState, newState);
        }
    }
    else if (VEHICLE_ACT_STATE::REMOVED == prevState && isStateInstalled(newState)) {
        if (_listener) {
            _listener->onActionVehicleInstalled(id, prevState, newState);
        }
    }
    else {
        if (_listener) {
            _listener->onActionVehicleETC(id, prevState, newState);
        }
    }
}

void B1AMHSVehicleStateRepository::setVehicleAvailState(int32 id, VEHICLE_AVAIL_STATE newState)
{
    VEHICLE_AVAIL_STATE prevState;
    if (setState(id, newState, &prevState) != true) {
        return;
    }
    if (VEHICLE_AVAIL_STATE::IN_SERVICE == newState) {
        if (_listener) {
            _listener->onActionVehicleInService(id, prevState, newState);
        }
    }
    else if (VEHICLE_AVAIL_STATE::OUT_OF_SERVICE == newState) {
        if (_listener) {
            _listener->onActionVehicleOutOfService(id, prevState, newState);
        }
    }
}

VEHICLE_ACT_STATE B1AMHSVehicleStateRepository::vehicleActivityState(int32 id) const
{
    B1AutoLock al(*_lock);
    auto itr = _states.find(id);
    return itr != _states.end() ? itr->second._activityState : VEHICLE_ACT_STATE::NONE;
}

VEHICLE_AVAIL_STATE B1AMHSVehicleStateRepository::vehicleAvailState(int32 id) const
{
    B1AutoLock al(*_lock);
    auto itr = _states.find(id);
    return itr != _states.end() ? itr->second._availState : VEHICLE_AVAIL_STATE::NONE;
}
