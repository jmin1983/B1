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

bool B1AMHSVehicleStateRepository::setVehicleActivityState(int32 id, VEHICLE_ACT_STATE newState)
{
    _lock->lock();
    auto itr = _states.find(id);
    if (itr == _states.end()) {
        assert(false);
        _lock->unlock();
        return false;
    }
    if (itr->second._activityState == newState) {
        _lock->unlock();
        return true;
    }

    const auto currentState = itr->second._activityState;
    if (VEHICLE_ACT_STATE::INSTALLED_ASSIGNED_ENROUTE == currentState && VEHICLE_ACT_STATE::INSTALLED_ASSIGNED_PARKED == newState) {
        itr->second._activityState = newState;
        _lock->unlock();
        B1LOG("onActionVehicleArrived: id[%d], state[%d]->[%d]", id, currentState, newState);
        if (_listener) {
            _listener->onActionVehicleArrived(id, currentState, newState);
        }
        return true;
    }
    else if (VEHICLE_ACT_STATE::INSTALLED_ASSIGNED_PARKED == currentState && VEHICLE_ACT_STATE::INSTALLED_ASSIGNED_ENROUTE == newState) {
        itr->second._activityState = newState;
        _lock->unlock();
        B1LOG("onActionVehicleDeparted: id[%d], state[%d]->[%d]", id, currentState, newState);
        if (_listener) {
            _listener->onActionVehicleDeparted(id, currentState, newState);
        }
        return true;
    }
    else if (VEHICLE_ACT_STATE::INSTALLED_ASSIGNED_PARKED == currentState && VEHICLE_ACT_STATE::INSTALLED_ASSIGNED_ACQUIRING == newState) {
        itr->second._activityState = newState;
        _lock->unlock();
        B1LOG("onActionVehicleAcquireStarted: id[%d], state[%d]->[%d]", id, currentState, newState);
        if (_listener) {
            _listener->onActionVehicleAcquireStarted(id, currentState, newState);
        }
        return true;
    }
    else if (VEHICLE_ACT_STATE::INSTALLED_ASSIGNED_ACQUIRING == currentState && VEHICLE_ACT_STATE::INSTALLED_ASSIGNED_PARKED == newState) {
        itr->second._activityState = newState;
        _lock->unlock();
        B1LOG("onActionVehicleAcquireCompleted: id[%d], state[%d]->[%d]", id, currentState, newState);
        if (_listener) {
            _listener->onActionVehicleAcquireCompleted(id, currentState, newState);
        }
        return true;
    }
    else if (VEHICLE_ACT_STATE::INSTALLED_ASSIGNED_PARKED == currentState && VEHICLE_ACT_STATE::INSTALLED_ASSIGNED_DEPOSITING == newState) {
        itr->second._activityState = newState;
        _lock->unlock();
        B1LOG("onActionVehicleDepositStarted: id[%d], state[%d]->[%d]", id, currentState, newState);
        if (_listener) {
            _listener->onActionVehicleDepositStarted(id, currentState, newState);
        }
        return true;
    }
    else if (VEHICLE_ACT_STATE::INSTALLED_ASSIGNED_DEPOSITING == currentState && VEHICLE_ACT_STATE::INSTALLED_ASSIGNED_PARKED == newState) {
        itr->second._activityState = newState;
        _lock->unlock();
        B1LOG("onActionVehicleDepositCompleted: id[%d], state[%d]->[%d]", id, currentState, newState);
        if (_listener) {
            _listener->onActionVehicleDepositCompleted(id, currentState, newState);
        }
        return true;
    }
    else if (VEHICLE_ACT_STATE::INSTALLED_ASSIGNED_ACQUIRING == currentState && VEHICLE_ACT_STATE::INSTALLED_ASSIGNED_DEPOSITING == newState) {
        itr->second._activityState = newState;
        _lock->unlock();
        B1LOG("onActionVehicleDepositStarted: id[%d], state[%d]->[%d]", id, currentState, newState);
        if (_listener) {
            _listener->onActionVehicleDepositStarted(id, currentState, newState);
        }
        return true;
    }
    else if (VEHICLE_ACT_STATE::INSTALLED_ASSIGNED_DEPOSITING == currentState && VEHICLE_ACT_STATE::INSTALLED_ASSIGNED_ACQUIRING == newState) {
        itr->second._activityState = newState;
        _lock->unlock();
        B1LOG("onActionVehicleAcquireStarted: id[%d], state[%d]->[%d]", id, currentState, newState);
        if (_listener) {
            _listener->onActionVehicleAcquireStarted(id, currentState, newState);
        }
        return true;
    }
    else if (isStateAssigned(currentState) && VEHICLE_ACT_STATE::INSTALLED_NOT_ASSIGNED == newState) {
        itr->second._activityState = newState;
        _lock->unlock();
        B1LOG("onActionVehicleUnassigned: id[%d], state[%d]->[%d]", id, currentState, newState);
        if (_listener) {
            _listener->onActionVehicleUnassigned(id, currentState, newState);
        }
        return true;
    }
    else if (VEHICLE_ACT_STATE::INSTALLED_NOT_ASSIGNED == currentState && isStateAssigned(newState)) {
        itr->second._activityState = newState;
        _lock->unlock();
        B1LOG("onActionVehicleAssigned: id[%d], state[%d]->[%d]", id, currentState, newState);
        if (_listener) {
            _listener->onActionVehicleAssigned(id, currentState, newState);
        }
        return true;
    }
    else if (isStateInstalled(currentState) && VEHICLE_ACT_STATE::REMOVED == newState) {
        itr->second._activityState = newState;
        _lock->unlock();
        B1LOG("onActionVehicleRemoved: id[%d], state[%d]->[%d]", id, currentState, newState);
        if (_listener) {
            _listener->onActionVehicleRemoved(id, currentState, newState);
        }
        return true;
    }
    else if (VEHICLE_ACT_STATE::REMOVED == currentState && isStateInstalled(newState)) {
        itr->second._activityState = newState;
        _lock->unlock();
        B1LOG("onActionVehicleInstalled: id[%d], state[%d]->[%d]", id, currentState, newState);
        if (_listener) {
            _listener->onActionVehicleInstalled(id, currentState, newState);
        }
        return true;
    }

    _lock->unlock();
    if (_listener) {
        _listener->onActionVehicleETC(id, currentState, newState);
    }
    return false;
}

void B1AMHSVehicleStateRepository::setVehicleAvailState(int32 id, VEHICLE_AVAIL_STATE newState)
{
    VEHICLE_AVAIL_STATE prevState;
    {
        B1AutoLock al(*_lock);
        auto itr = _states.find(id);
        if (itr == _states.end()) {
            return;
        }
        if (itr->second._availState == newState) {
            return;
        }
        prevState = itr->second._availState;
        itr->second._availState = newState;
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
    for (const auto& state : states) {
        _states.insert(std::make_pair(state.first, state.second));
    }
    _listener = listener;
    return true;
}

void B1AMHSVehicleStateRepository::finalize()
{
    _listener = NULL;
    _states.clear();
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
