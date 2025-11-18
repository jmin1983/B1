//
// B1AMHSCarrierTransferStateRepository.h
//
// Library: B1SEM
// Package: B1SEM
// Module:  B1SEM
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1SEM_AMHS_CARRIER_TRANSFER_STATE_REPOSITORY_H
#define _B1SEM_AMHS_CARRIER_TRANSFER_STATE_REPOSITORY_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <map>

namespace BnD {
    namespace B1AMHSSEM {
        enum class CARRIER_STATE;
        enum class TRANSFER_COMMAND_STATE;
    };
    class B1AMHSCarrierTransferState;
    class B1AMHSCarrierTransferStateRepositoryListener;
    class B1RecursiveLock;
    class B1AMHSCarrierTransferStateRepository {
    public:
        B1AMHSCarrierTransferStateRepository();
        virtual ~B1AMHSCarrierTransferStateRepository();
    protected:
        B1AMHSCarrierTransferStateRepositoryListener* _listener;
        std::shared_ptr<B1RecursiveLock> _lock;
        std::map<int64, std::shared_ptr<B1AMHSCarrierTransferState> > _data;  //  map<task_id, data>
    protected:
        virtual bool implInitialize() { return true; }
        virtual void implFinalize() {}
    protected:
        bool setCarrierState(int64 id, B1AMHSSEM::CARRIER_STATE newState);
        bool setTransferCommandState(int64 id, B1AMHSSEM::TRANSFER_COMMAND_STATE newState);
        bool isCarrierStateInstalled(B1AMHSSEM::CARRIER_STATE state) const;
        bool isTransferStateNotActive(B1AMHSSEM::TRANSFER_COMMAND_STATE state) const;
        bool isTransferStateActive(B1AMHSSEM::TRANSFER_COMMAND_STATE state) const;
        bool addData(int64 id, std::shared_ptr<B1AMHSCarrierTransferState> data);
        void removeData(int64 id);
    public:
        bool initialize(std::map<int64, std::shared_ptr<B1AMHSCarrierTransferState> >&& data, B1AMHSCarrierTransferStateRepositoryListener* listener);
        void finalize();
        bool setCarrierStateWaitIn(int64 id);
        bool setCarrierStateTransferring(int64 id);
        bool setCarrierStateCompleted(int64 id);
        bool setCarrierStateWaitOut(int64 id);
        bool setCarrierStateNone(int64 id);
        bool setCarrierStateAlternate(int64 id);
        bool setTransferCommandStateQueued(int64 id);
        bool setTransferCommandStateWaiting(int64 id);
        bool setTransferCommandStateTransferring(int64 id);
        bool setTransferCommandStateCanceling(int64 id);
        bool setTransferCommandStateNone(int64 id);
        bool setTransferCommandStatePaused(int64 id);
        bool setTransferCommandStateAborting(int64 id);
        std::shared_ptr<B1AMHSCarrierTransferState> findCarrierTransferState(int64 id) const;
    };
}   //  !BnD

#endif  // !_B1SEM_AMHS_CARRIER_TRANSFER_STATE_REPOSITORY_H
