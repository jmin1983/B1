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

B1AMHSCarrierTransferStateRepository::B1AMHSCarrierTransferStateRepository(B1AMHSCarrierTransferStateRepositoryListener* listener)
    : _listener(listener)
    , _lock(new B1RecursiveLock())
    , _data()
{
}

B1AMHSCarrierTransferStateRepository::~B1AMHSCarrierTransferStateRepository()
{
}

bool B1AMHSCarrierTransferStateRepository::setState(int32 id, CARRIER_STATE newState, CARRIER_STATE* prevState)
{
    B1AutoLock al(*_lock);
    auto itr = _data.find(id);
    if (itr == _data.end()) {
        return false;
    }
    if (itr->second->carrierState() == newState) {
        return false;
    }
    *prevState = itr->second->carrierState();
    itr->second->setCarrierState(newState);
    return true;
}

bool B1AMHSCarrierTransferStateRepository::setState(int32 id, TRANSFER_COMMAND_STATE newState, TRANSFER_COMMAND_STATE* prevState)
{
    B1AutoLock al(*_lock);
    auto itr = _data.find(id);
    if (itr == _data.end()) {
        return false;
    }
    if (itr->second->transferCommandState() == newState) {
        return false;
    }
    *prevState = itr->second->transferCommandState();
    itr->second->setTransferCommandState(newState);
    return true;
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

bool B1AMHSCarrierTransferStateRepository::initialize()
{
    return implInitialize();
}

void B1AMHSCarrierTransferStateRepository::finalize()
{
    implFinalize();
    _data.clear();
}

bool B1AMHSCarrierTransferStateRepository::addData(int32 id, std::shared_ptr<B1AMHSCarrierTransferState> data)
{
    B1AutoLock al(*_lock);
    if (_data.find(id) != _data.end()) {
        return false;
    }
    _data.insert(std::make_pair(id, data));
    return true;
}

void B1AMHSCarrierTransferStateRepository::removeData(int32 id)
{
    B1AutoLock al(*_lock);
    _data.erase(id);
}

void B1AMHSCarrierTransferStateRepository::setCarrierState(int32 id, CARRIER_STATE newState)
{
    CARRIER_STATE prevState;
    if (setState(id, newState, &prevState) != true) {
        return;
    }
    if (CARRIER_STATE::NONE == prevState && CARRIER_STATE::INSTALLED_WAIT_IN == newState) {
        if (_listener) {
            _listener->onActionCarrierWaitIn(id, prevState, newState);
        }
    }
    else if (CARRIER_STATE::INSTALLED_WAIT_IN == prevState && CARRIER_STATE::INSTALLED_TRANSFERRING == newState) {
        if (_listener) {
            _listener->onActionCarrierTransferring(id, prevState, newState);
        }
    }
    else if (CARRIER_STATE::INSTALLED_TRANSFERRING == prevState && CARRIER_STATE::INSTALLED_STORED_COMPLETED == newState) {
        if (_listener) {
            _listener->onActionCarrierStored(id, prevState, newState);
        }
    }
    else if (CARRIER_STATE::INSTALLED_STORED_COMPLETED == prevState && CARRIER_STATE::INSTALLED_TRANSFERRING == newState) {
        if (_listener) {
            _listener->onActionCarrierTransferring(id, prevState, newState);
        }
    }
    else if (CARRIER_STATE::INSTALLED_TRANSFERRING == prevState && CARRIER_STATE::INSTALLED_WAIT_OUT == newState) {
        if (_listener) {
            _listener->onActionCarrierWaitOut(id, prevState, newState);
        }
    }
    else if (CARRIER_STATE::INSTALLED_WAIT_OUT == prevState && CARRIER_STATE::NONE == newState) {
        if (_listener) {
            _listener->onActionCarrierRemoved(id, prevState, newState);
        }
    }
    else if (CARRIER_STATE::INSTALLED_TRANSFERRING == prevState && CARRIER_STATE::INSTALLED_STORED_ALTERNATE == newState) {
        if (_listener) {
            _listener->onActionCarrierStoredAlt(id, prevState, newState);
        }
    }
    else if (CARRIER_STATE::INSTALLED_STORED_ALTERNATE == prevState && CARRIER_STATE::INSTALLED_TRANSFERRING == newState) {
        if (_listener) {
            _listener->onActionCarrierResumed(id, prevState, newState);
        }
    }
    else if (CARRIER_STATE::NONE == prevState && CARRIER_STATE::INSTALLED_STORED_COMPLETED == newState) {
        if (_listener) {
            _listener->onActionCarrierInstallCompleted(id, prevState, newState);
        }
    }
    else if (isCarrierStateInstalled(prevState) && CARRIER_STATE::NONE == newState) {
        if (_listener) {
            _listener->onActionCarrierRemoveCompleted(id, prevState, newState);
        }
    }
    else if (CARRIER_STATE::INSTALLED_STORED_ALTERNATE == prevState && CARRIER_STATE::INSTALLED_STORED_COMPLETED == newState) {
        if (_listener) {
            _listener->onActionCarrierStored(id, prevState, newState);
        }
    }
    else if (CARRIER_STATE::NONE == prevState && CARRIER_STATE::INSTALLED_WAIT_OUT == newState) {
        if (_listener) {
            _listener->onActionCarrierWaitOut(id, prevState, newState);
        }
    }
    else if (CARRIER_STATE::NONE == prevState && CARRIER_STATE::INSTALLED_TRANSFERRING == newState) {
        if (_listener) {
            _listener->onActionCarrierTransferring(id, prevState, newState);
        }
    }
    else if (CARRIER_STATE::INSTALLED_TRANSFERRING == prevState && CARRIER_STATE::NONE == newState) {
        if (_listener) {
            _listener->onActionCarrierRemoved(id, prevState, newState);
        }
    }
    else {
        if (_listener) {
            _listener->onActionCarrierETC(id, prevState, newState);
        }
    }
}

void B1AMHSCarrierTransferStateRepository::setTransferCommandState(int32 id, TRANSFER_COMMAND_STATE newState, void* param)
{
    TRANSFER_COMMAND_STATE prevState;
    if (setState(id, newState, &prevState) != true) {
        return;
    }
    if (TRANSFER_COMMAND_STATE::NOT_ACTIVE_QUEUED == prevState && TRANSFER_COMMAND_STATE::NOT_ACTIVE_WAITING == newState) {
        if (_listener) {
            _listener->onActionTransferInitiated(id, prevState, newState);
        }
    }
    else if (TRANSFER_COMMAND_STATE::NOT_ACTIVE_WAITING == prevState && TRANSFER_COMMAND_STATE::ACTIVE_TRANSFERRING == newState) {
        if (_listener) {
            _listener->onActionTransferring(id, prevState, newState);
        }
    }
    else if (TRANSFER_COMMAND_STATE::CANCELING == newState) {
        if (isTransferStateNotActive(prevState)) {
            if (_listener) {
                _listener->onActionTransferCancelInitiated(id, prevState, newState);
            }
            setTransferCommandState(id, TRANSFER_COMMAND_STATE::NONE, param);
        }
        else {
            if (_listener) {
                _listener->onActionTransferCancelFailed(id, prevState, newState);
            }
            setTransferCommandState(id, prevState, param);
        }
    }
    else if (TRANSFER_COMMAND_STATE::CANCELING == prevState && TRANSFER_COMMAND_STATE::NONE == newState) {
        if (_listener) {
            _listener->onActionTransferCancelCompleted(id, prevState, newState);
        }
    }
    else if (TRANSFER_COMMAND_STATE::ACTIVE_TRANSFERRING == prevState && TRANSFER_COMMAND_STATE::ACTIVE_PAUSED == newState) {
        if (_listener) {
            _listener->onActionTransferPaused(id, prevState, newState);
        }
    }
    else if (TRANSFER_COMMAND_STATE::ACTIVE_PAUSED == prevState && TRANSFER_COMMAND_STATE::ACTIVE_TRANSFERRING == newState) {
        if (_listener) {
            _listener->onActionTransferResumed(id, prevState, newState);
        }
    }
    else if (TRANSFER_COMMAND_STATE::ABORTING == newState) {
        if (isTransferStateActive(prevState)) {
            if (_listener) {
                _listener->onActionTransferAbortInitiated(id, prevState, newState);
            }
            setTransferCommandState(id, TRANSFER_COMMAND_STATE::NONE, param);
        }
        else {
            if (_listener) {
                _listener->onActionTransferAbortFailed(id, prevState, newState);
            }
            setTransferCommandState(id, prevState, param);
        }
    }
    else if (TRANSFER_COMMAND_STATE::ABORTING == prevState && TRANSFER_COMMAND_STATE::NONE == newState) {
        if (_listener) {
            _listener->onActionTransferAbortCompleted(id, prevState, newState);
        }
    }
    else if (isTransferStateActive(prevState) && TRANSFER_COMMAND_STATE::NONE == newState) {
        if (_listener) {
            _listener->onActionTransferCompleted(id, prevState, newState, param);
        }
    }
    else if (TRANSFER_COMMAND_STATE::CANCELING == prevState && TRANSFER_COMMAND_STATE::ACTIVE_TRANSFERRING == newState) {
        if (_listener) {
            _listener->onActionTransferring(id, prevState, newState);
        }
    }
    else if (TRANSFER_COMMAND_STATE::NOT_ACTIVE_QUEUED == prevState && TRANSFER_COMMAND_STATE::ACTIVE_TRANSFERRING == newState) {
        if (_listener) {
            _listener->onActionTransferring(id, prevState, newState);
        }
    }
    else {
        if (_listener) {
            _listener->onActionTransferETC(id, prevState, newState);
        }
    }
}

CARRIER_STATE B1AMHSCarrierTransferStateRepository::carrierState(int32 id) const
{
    B1AutoLock al(*_lock);
    auto itr = _data.find(id);
    return itr != _data.end() ? itr->second->carrierState() : CARRIER_STATE::NONE;
}

TRANSFER_COMMAND_STATE B1AMHSCarrierTransferStateRepository::transferState(int32 id) const
{
    B1AutoLock al(*_lock);
    auto itr = _data.find(id);
    return itr != _data.end() ? itr->second->transferCommandState() : TRANSFER_COMMAND_STATE::NONE;
}
