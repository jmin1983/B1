//
// B1AMHSPortStateRepository.cpp
//
// Library: B1SEM
// Package: B1SEM
// Module:  B1SEM
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1SEM.h"
#include "B1AMHSPortStateRepository.h"
#include "B1AMHSPortStateRepositoryListener.h"
#include "B1AMHSStates.h"

#include <B1Base/B1Lock.h>

using namespace BnD;
using namespace B1AMHSSEM;

B1AMHSPortStateRepository::B1AMHSPortStateRepository()
    : _listener(NULL)
    , _lock(new B1Lock())
    , _states()
{
}

B1AMHSPortStateRepository::~B1AMHSPortStateRepository()
{
}

bool B1AMHSPortStateRepository::setState(int32 id, PORT_STATE newState, PORT_STATE* prevState)
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

void B1AMHSPortStateRepository::setPortState(int32 id, PORT_STATE newState)
{
    PORT_STATE prevState;
    if (setState(id, newState, &prevState) != true) {
        return;
    }
    if (PORT_STATE::IN_SERVICE == newState) {
        if (_listener) {
            _listener->onActionPortInService(id, prevState, newState);
        }
    }
    else if (PORT_STATE::OUT_OF_SERVICE == newState) {
        if (_listener) {
            _listener->onActionPortOutOfService(id, prevState, newState);
        }
    }
}

bool B1AMHSPortStateRepository::initialize(std::map<int32, B1AMHSSEM::PORT_STATE>&& states, B1AMHSPortStateRepositoryListener* listener)
{
    _listener = listener;
    _states.swap(states);
    return true;
}

void B1AMHSPortStateRepository::finalize()
{
    _listener = NULL;
    _states.clear();
}

void B1AMHSPortStateRepository::setPortStateInService(int32 id)
{
    setPortState(id, PORT_STATE::IN_SERVICE);
}

void B1AMHSPortStateRepository::setPortStateOutOfService(int32 id)
{
    setPortState(id, PORT_STATE::OUT_OF_SERVICE);
}

bool B1AMHSPortStateRepository::isPortStateInService(int32 id) const
{
    return portState(id) == PORT_STATE::IN_SERVICE;
}

PORT_STATE B1AMHSPortStateRepository::portState(int32 id) const
{
    B1AutoLock al(*_lock);
    auto itr = _states.find(id);
    return itr != _states.end() ? itr->second : PORT_STATE::NONE;
}
