//
// B1AMHSCarrierTransferStateRepository.cpp
//
// Library: B1SEM
// Package: B1SEM
// Module:  B1SEM
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1SEM.h"
#include "B1AMHSCarrierTransferStateRepository.h"
#include "B1AMHSCarrierTransferState.h"
#include "B1AMHSCarrierTransferStateRepositoryListener.h"
#include "B1AMHSStates.h"

#include <B1Base/B1Lock.h>

using namespace BnD;
using namespace B1AMHSSEM;

B1AMHSCarrierTransferStateRepository::B1AMHSCarrierTransferStateRepository()
    : _listener(NULL)
    , _lock(new B1RecursiveLock())
    , _data()
{
}

B1AMHSCarrierTransferStateRepository::~B1AMHSCarrierTransferStateRepository()
{
}

bool B1AMHSCarrierTransferStateRepository::setCarrierState(int64 id, CARRIER_STATE newState)
{
    _lock->lock();
    auto itr = _data.find(id);
    if (itr == _data.end()) {
        assert(false);
        _lock->unlock();
        return false;
    }
    if (itr->second->carrierState() == newState) {
        _lock->unlock();
        return true;
    }

    const auto currentState = itr->second->carrierState();
    if (CARRIER_STATE::NONE == currentState && CARRIER_STATE::INSTALLED_WAIT_IN == newState) {
        itr->second->setCarrierState(newState);
        _lock->unlock();
        B1LOG("onActionCarrierWaitIn: id[%lld], state[%d]->[%d]", id, currentState, newState);
        if (_listener) {
            _listener->onActionCarrierWaitIn(id, currentState, newState);
        }
        return true;
    }
    else if (CARRIER_STATE::INSTALLED_WAIT_IN == currentState && CARRIER_STATE::INSTALLED_TRANSFERRING == newState) {
        itr->second->setCarrierState(newState);
        _lock->unlock();
        B1LOG("onActionCarrierTransferring: id[%lld], state[%d]->[%d]", id, currentState, newState);
        if (_listener) {
            _listener->onActionCarrierTransferring(id, currentState, newState);
        }
        return true;
    }
    else if (CARRIER_STATE::INSTALLED_TRANSFERRING == currentState && CARRIER_STATE::INSTALLED_STORED_COMPLETED == newState) {
        itr->second->setCarrierState(newState);
        _lock->unlock();
        B1LOG("onActionCarrierStored: id[%lld], state[%d]->[%d]", id, currentState, newState);
        if (_listener) {
            _listener->onActionCarrierStored(id, currentState, newState);
        }
        return true;
    }
    else if (CARRIER_STATE::INSTALLED_STORED_COMPLETED == currentState && CARRIER_STATE::INSTALLED_TRANSFERRING == newState) {
        itr->second->setCarrierState(newState);
        _lock->unlock();
        B1LOG("onActionCarrierTransferring: id[%lld], state[%d]->[%d]", id, currentState, newState);
        if (_listener) {
            _listener->onActionCarrierTransferring(id, currentState, newState);
        }
        return true;
    }
    else if (CARRIER_STATE::INSTALLED_TRANSFERRING == currentState && CARRIER_STATE::INSTALLED_WAIT_OUT == newState) {
        itr->second->setCarrierState(newState);
        _lock->unlock();
        B1LOG("onActionCarrierWaitOut: id[%lld], state[%d]->[%d]", id, currentState, newState);
        if (_listener) {
            _listener->onActionCarrierWaitOut(id, currentState, newState);
        }
        return true;
    }
    else if (CARRIER_STATE::INSTALLED_WAIT_OUT == currentState && CARRIER_STATE::NONE == newState) {
        itr->second->setCarrierState(newState);
        _lock->unlock();
        B1LOG("onActionCarrierRemoved: id[%lld], state[%d]->[%d]", id, currentState, newState);
        if (_listener) {
            _listener->onActionCarrierRemoved(id, currentState, newState);
        }
        return true;
    }
    else if (CARRIER_STATE::INSTALLED_TRANSFERRING == currentState && CARRIER_STATE::INSTALLED_STORED_ALTERNATE == newState) {
        itr->second->setCarrierState(newState);
        _lock->unlock();
        B1LOG("onActionCarrierStoredAlt: id[%lld], state[%d]->[%d]", id, currentState, newState);
        if (_listener) {
            _listener->onActionCarrierStoredAlt(id, currentState, newState);
        }
        return true;
    }
    else if (CARRIER_STATE::INSTALLED_STORED_ALTERNATE == currentState && CARRIER_STATE::INSTALLED_TRANSFERRING == newState) {
        itr->second->setCarrierState(newState);
        _lock->unlock();
        B1LOG("onActionCarrierResumed: id[%lld], state[%d]->[%d]", id, currentState, newState);
        if (_listener) {
            _listener->onActionCarrierResumed(id, currentState, newState);
        }
        return true;
    }
    else if (CARRIER_STATE::NONE == currentState && CARRIER_STATE::INSTALLED_STORED_COMPLETED == newState) {
        itr->second->setCarrierState(newState);
        _lock->unlock();
        B1LOG("onActionCarrierInstallCompleted: id[%lld], state[%d]->[%d]", id, currentState, newState);
        if (_listener) {
            _listener->onActionCarrierInstallCompleted(id, currentState, newState);
        }
        return true;
    }
    else if (isCarrierStateInstalled(currentState) && CARRIER_STATE::NONE == newState) {
        itr->second->setCarrierState(newState);
        _lock->unlock();
        B1LOG("onActionCarrierRemoveCompleted: id[%lld], state[%d]->[%d]", id, currentState, newState);
        if (_listener) {
            _listener->onActionCarrierRemoveCompleted(id, currentState, newState);
        }
        return true;
    }
    else if (CARRIER_STATE::INSTALLED_STORED_ALTERNATE == currentState && CARRIER_STATE::INSTALLED_STORED_COMPLETED == newState) {
        itr->second->setCarrierState(newState);
        _lock->unlock();
        B1LOG("onActionCarrierStored: id[%lld], state[%d]->[%d]", id, currentState, newState);
        if (_listener) {
            _listener->onActionCarrierStored(id, currentState, newState);
        }
        return true;
    }
    else if (CARRIER_STATE::NONE == currentState && CARRIER_STATE::INSTALLED_WAIT_OUT == newState) {
        itr->second->setCarrierState(newState);
        _lock->unlock();
        B1LOG("onActionCarrierWaitOut: id[%lld], state[%d]->[%d]", id, currentState, newState);
        if (_listener) {
            _listener->onActionCarrierWaitOut(id, currentState, newState);
        }
        return true;
    }
    else if (CARRIER_STATE::NONE == currentState && CARRIER_STATE::INSTALLED_TRANSFERRING == newState) {
        itr->second->setCarrierState(newState);
        _lock->unlock();
        B1LOG("onActionCarrierTransferring: id[%lld], state[%d]->[%d]", id, currentState, newState);
        if (_listener) {
            _listener->onActionCarrierTransferring(id, currentState, newState);
        }
        return true;
    }
    else if (CARRIER_STATE::INSTALLED_TRANSFERRING == currentState && CARRIER_STATE::NONE == newState) {
        itr->second->setCarrierState(newState);
        _lock->unlock();
        B1LOG("onActionCarrierRemoved: id[%lld], state[%d]->[%d]", id, currentState, newState);
        if (_listener) {
            _listener->onActionCarrierRemoved(id, currentState, newState);
        }
        return true;
    }

    _lock->unlock();
    if (_listener) {
        _listener->onActionCarrierETC(id, currentState, newState);
    }
    return false;
}

bool B1AMHSCarrierTransferStateRepository::setTransferCommandState(int64 id, TRANSFER_COMMAND_STATE newState)
{
    _lock->lock();
    auto itr = _data.find(id);
    if (itr == _data.end()) {
        assert(false);
        _lock->unlock();
        return false;
    }
    if (itr->second->transferCommandState() == newState) {
        _lock->unlock();
        return true;
    }

    const auto currentState = itr->second->transferCommandState();
    if (TRANSFER_COMMAND_STATE::NONE == currentState && TRANSFER_COMMAND_STATE::NOT_ACTIVE_QUEUED == newState) {
        itr->second->setTransferCommandState(newState);
        _lock->unlock();
        B1LOG("onActionTransferQueued: id[%lld], state[%d]->[%d]", id, currentState, newState);
        if (_listener) {
            _listener->onActionTransferQueued(id, currentState, newState);
        }
        return true;
    }
    else if (TRANSFER_COMMAND_STATE::NOT_ACTIVE_QUEUED == currentState && TRANSFER_COMMAND_STATE::NOT_ACTIVE_WAITING == newState) {
        itr->second->setTransferCommandState(newState);
        _lock->unlock();
        B1LOG("onActionTransferInitiated: id[%lld], state[%d]->[%d]", id, currentState, newState);
        if (_listener) {
            _listener->onActionTransferInitiated(id, currentState, newState);
        }
        return true;
    }
    else if (TRANSFER_COMMAND_STATE::NOT_ACTIVE_WAITING == currentState && TRANSFER_COMMAND_STATE::ACTIVE_TRANSFERRING == newState) {
        itr->second->setTransferCommandState(newState);
        _lock->unlock();
        B1LOG("onActionTransferring: id[%lld], state[%d]->[%d]", id, currentState, newState);
        if (_listener) {
            _listener->onActionTransferring(id, currentState, newState);
        }
        return true;
    }
    else if (isTransferStateNotActive(currentState) && TRANSFER_COMMAND_STATE::CANCELING == newState) {
        itr->second->setTransferCommandState(newState);
        _lock->unlock();
        B1LOG("onActionTransferCancelInitiated: id[%lld], state[%d]->[%d]", id, currentState, newState);
        if (_listener) {
            _listener->onActionTransferCancelInitiated(id, currentState, newState);
        }
        return true;
    }
    else if (TRANSFER_COMMAND_STATE::CANCELING == currentState && isTransferStateNotActive(newState)) {
        itr->second->setTransferCommandState(newState);
        _lock->unlock();
        B1LOG("onActionTransferCancelFailed: id[%lld], state[%d]->[%d]", id, currentState, newState);
        if (_listener) {
            _listener->onActionTransferCancelFailed(id, currentState, newState);
        }
        return true;
    }
    else if (TRANSFER_COMMAND_STATE::CANCELING == currentState && TRANSFER_COMMAND_STATE::NONE == newState) {
        itr->second->setTransferCommandState(newState);
        _lock->unlock();
        B1LOG("onActionTransferCancelCompleted: id[%lld], state[%d]->[%d]", id, currentState, newState);
        if (_listener) {
            _listener->onActionTransferCancelCompleted(id, currentState, newState);
        }
        return true;
    }
    else if (TRANSFER_COMMAND_STATE::ACTIVE_TRANSFERRING == currentState && TRANSFER_COMMAND_STATE::ACTIVE_PAUSED == newState) {
        itr->second->setTransferCommandState(newState);
        _lock->unlock();
        B1LOG("onActionTransferPaused: id[%lld], state[%d]->[%d]", id, currentState, newState);
        if (_listener) {
            _listener->onActionTransferPaused(id, currentState, newState);
        }
        return true;
    }
    else if (TRANSFER_COMMAND_STATE::ACTIVE_PAUSED == currentState && TRANSFER_COMMAND_STATE::ACTIVE_TRANSFERRING == newState) {
        itr->second->setTransferCommandState(newState);
        _lock->unlock();
        B1LOG("onActionTransferResumed: id[%lld], state[%d]->[%d]", id, currentState, newState);
        if (_listener) {
            _listener->onActionTransferResumed(id, currentState, newState);
        }
        return true;
    }
    else if (isTransferStateActive(currentState) && TRANSFER_COMMAND_STATE::ABORTING == newState) {
        itr->second->setTransferCommandState(newState);
        _lock->unlock();
        B1LOG("onActionTransferAbortInitiated: id[%lld], state[%d]->[%d]", id, currentState, newState);
        if (_listener) {
            _listener->onActionTransferAbortInitiated(id, currentState, newState);
        }
        return true;
    }

    else if (TRANSFER_COMMAND_STATE::ABORTING == currentState && TRANSFER_COMMAND_STATE::NONE == newState) {
        itr->second->setTransferCommandState(newState);
        _lock->unlock();
        B1LOG("onActionTransferAbortCompleted: id[%lld], state[%d]->[%d]", id, currentState, newState);
        if (_listener) {
            _listener->onActionTransferAbortCompleted(id, currentState, newState);
        }
        return true;
    }
    else if (isTransferStateActive(currentState) && TRANSFER_COMMAND_STATE::NONE == newState) {
        itr->second->setTransferCommandState(newState);
        _lock->unlock();
        B1LOG("onActionTransferCompleted: id[%lld], state[%d]->[%d]", id, currentState, newState);
        if (_listener) {
            _listener->onActionTransferCompleted(id, currentState, newState);
        }
        return true;
    }
    else if (TRANSFER_COMMAND_STATE::ABORTING == currentState && isTransferStateActive(newState)) {
        itr->second->setTransferCommandState(newState);
        _lock->unlock();
        B1LOG("onActionTransferAbortFailed: id[%lld], state[%d]->[%d]", id, currentState, newState);
        if (_listener) {
            _listener->onActionTransferAbortFailed(id, currentState, newState);
        }
        return true;
    }
    else if (TRANSFER_COMMAND_STATE::CANCELING == currentState && TRANSFER_COMMAND_STATE::ACTIVE_TRANSFERRING == newState) {
        itr->second->setTransferCommandState(newState);
        _lock->unlock();
        B1LOG("onActionTransferring: id[%lld], state[%d]->[%d]", id, currentState, newState);
        if (_listener) {
            _listener->onActionTransferring(id, currentState, newState);
        }
        return true;
    }
    else if (TRANSFER_COMMAND_STATE::NOT_ACTIVE_QUEUED == currentState && TRANSFER_COMMAND_STATE::ACTIVE_TRANSFERRING == newState) {
        itr->second->setTransferCommandState(newState);
        _lock->unlock();
        B1LOG("onActionTransferring: id[%lld], state[%d]->[%d]", id, currentState, newState);
        if (_listener) {
            _listener->onActionTransferring(id, currentState, newState);
        }
        return true;
    }

    _lock->unlock();
    if (_listener) {
        _listener->onActionTransferETC(id, currentState, newState);
    }
    return false;
}

bool B1AMHSCarrierTransferStateRepository::isCarrierStateInstalled(CARRIER_STATE state) const
{
    return CARRIER_STATE::INSTALLED_WAIT_IN == state ||
           CARRIER_STATE::INSTALLED_WAIT_OUT == state ||
           CARRIER_STATE::INSTALLED_TRANSFERRING == state ||
           CARRIER_STATE::INSTALLED_STORED_COMPLETED == state ||
           CARRIER_STATE::INSTALLED_STORED_ALTERNATE == state;
}

bool B1AMHSCarrierTransferStateRepository::isTransferStateNotActive(TRANSFER_COMMAND_STATE state) const
{
    return TRANSFER_COMMAND_STATE::NOT_ACTIVE_QUEUED == state ||
           TRANSFER_COMMAND_STATE::NOT_ACTIVE_WAITING == state;
}

bool B1AMHSCarrierTransferStateRepository::isTransferStateActive(TRANSFER_COMMAND_STATE state) const
{
    return TRANSFER_COMMAND_STATE::ACTIVE_TRANSFERRING == state ||
           TRANSFER_COMMAND_STATE::ACTIVE_PAUSED == state;
}

bool B1AMHSCarrierTransferStateRepository::addData(int64 id, std::shared_ptr<B1AMHSCarrierTransferState> data)
{
    B1AutoLock al(*_lock);
    if (_data.find(id) != _data.end()) {
        return false;
    }
    _data.insert(std::make_pair(id, data));
    return true;
}

void B1AMHSCarrierTransferStateRepository::removeData(int64 id)
{
    B1AutoLock al(*_lock);
    _data.erase(id);
}

bool B1AMHSCarrierTransferStateRepository::initialize(std::map<int64, std::shared_ptr<B1AMHSCarrierTransferState> >&& data, B1AMHSCarrierTransferStateRepositoryListener* listener)
{
    _data.swap(data);
    _listener = listener;
    return implInitialize();
}

void B1AMHSCarrierTransferStateRepository::finalize()
{
    implFinalize();
    _data.clear();
    _listener = NULL;
}

bool B1AMHSCarrierTransferStateRepository::setCarrierStateWaitIn(int64 id)
{
    return setCarrierState(id, CARRIER_STATE::INSTALLED_WAIT_IN);
}

bool B1AMHSCarrierTransferStateRepository::setCarrierStateTransferring(int64 id)
{
    return setCarrierState(id, CARRIER_STATE::INSTALLED_TRANSFERRING);
}

bool B1AMHSCarrierTransferStateRepository::setCarrierStateCompleted(int64 id)
{
    return setCarrierState(id, CARRIER_STATE::INSTALLED_STORED_COMPLETED);
}

bool B1AMHSCarrierTransferStateRepository::setCarrierStateWaitOut(int64 id)
{
    return setCarrierState(id, CARRIER_STATE::INSTALLED_WAIT_OUT);
}

bool B1AMHSCarrierTransferStateRepository::setCarrierStateNone(int64 id)
{
    return setCarrierState(id, CARRIER_STATE::NONE);
}

bool B1AMHSCarrierTransferStateRepository::setCarrierStateAlternate(int64 id)
{
    return setCarrierState(id, CARRIER_STATE::INSTALLED_STORED_ALTERNATE);
}

bool B1AMHSCarrierTransferStateRepository::setTransferCommandStateQueued(int64 id)
{
    return setTransferCommandState(id, TRANSFER_COMMAND_STATE::NOT_ACTIVE_QUEUED);
}

bool B1AMHSCarrierTransferStateRepository::setTransferCommandStateWaiting(int64 id)
{
    return setTransferCommandState(id, TRANSFER_COMMAND_STATE::NOT_ACTIVE_WAITING);
}

bool B1AMHSCarrierTransferStateRepository::setTransferCommandStateTransferring(int64 id)
{
    return setTransferCommandState(id, TRANSFER_COMMAND_STATE::ACTIVE_TRANSFERRING);
}

bool B1AMHSCarrierTransferStateRepository::setTransferCommandStateCanceling(int64 id)
{
    return setTransferCommandState(id, TRANSFER_COMMAND_STATE::CANCELING);
}

bool B1AMHSCarrierTransferStateRepository::setTransferCommandStateNone(int64 id)
{
    return setTransferCommandState(id, TRANSFER_COMMAND_STATE::NONE);
}

bool B1AMHSCarrierTransferStateRepository::setTransferCommandStatePaused(int64 id)
{
    return setTransferCommandState(id, TRANSFER_COMMAND_STATE::ACTIVE_PAUSED);
}

bool B1AMHSCarrierTransferStateRepository::setTransferCommandStateAborting(int64 id)
{
    return setTransferCommandState(id, TRANSFER_COMMAND_STATE::ABORTING);
}

auto B1AMHSCarrierTransferStateRepository::findCarrierTransferState(int64 id) const -> std::shared_ptr<B1AMHSCarrierTransferState>
{
    B1AutoLock al(*_lock);
    auto itr = _data.find(id);
    return itr != _data.end() ? itr->second : std::shared_ptr<B1AMHSCarrierTransferState>();
}
