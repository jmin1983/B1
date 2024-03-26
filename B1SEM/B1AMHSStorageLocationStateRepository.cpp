//
// B1AMHSStorageLocationStateRepository.cpp
//
// Library: B1SEM
// Package: B1SEM
// Module:  B1SEM
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1SEM.h"
#include "B1AMHSStorageLocationStateRepository.h"
#include "B1AMHSStorageLocationStateRepositoryListener.h"
#include "B1AMHSStates.h"

#include <B1Base/B1Lock.h>

using namespace BnD;
using namespace B1AMHSSEM;

B1AMHSStorageLocationStateRepository::B1AMHSStorageLocationStateRepository()
    : _listener(NULL)
    , _lock(new B1Lock())
    , _states()
{
}

B1AMHSStorageLocationStateRepository::~B1AMHSStorageLocationStateRepository()
{
}

bool B1AMHSStorageLocationStateRepository::setState(int32 id, STORAGE_LOCATION_STATE newState, STORAGE_LOCATION_STATE* prevState)
{
    B1AutoLock al(*_lock);
    auto itr = _states.find(id);
    if (itr == _states.end()) {
        return false;
    }
    if (itr->second == newState) {
        return false;
    }
    *prevState = itr->second;
    itr->second = newState;
    return true;
}

bool B1AMHSStorageLocationStateRepository::initialize(const std::map<int32, B1AMHSSEM::STORAGE_LOCATION_STATE>& states, B1AMHSStorageLocationStateRepositoryListener* listener)
{
    _listener = listener;
    _states = states;
    return true;
}

void B1AMHSStorageLocationStateRepository::finalize()
{
    _listener = NULL;
    _states.clear();
}

void B1AMHSStorageLocationStateRepository::setStorageLocationState(int32 id, STORAGE_LOCATION_STATE newState)
{
    STORAGE_LOCATION_STATE prevState;
    if (setState(id, newState, &prevState) != true) {
        return;
    }
    if (_listener) {
        _listener->onActionStorageLocationStatusChanged(id, prevState, newState);
    }
}

STORAGE_LOCATION_STATE B1AMHSStorageLocationStateRepository::storageLocationState(int32 id) const
{
    B1AutoLock al(*_lock);
    auto itr = _states.find(id);
    return itr != _states.end() ? itr->second : STORAGE_LOCATION_STATE::NONE;
}
