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
        B1AMHSCarrierTransferStateRepository(B1AMHSCarrierTransferStateRepositoryListener* listener);
        virtual ~B1AMHSCarrierTransferStateRepository();
    protected:
        B1AMHSCarrierTransferStateRepositoryListener* _listener;
        std::shared_ptr<B1RecursiveLock> _lock;
        std::map<int32, std::shared_ptr<B1AMHSCarrierTransferState> > _data;  //  map<task_id, data>
    protected:
        virtual bool implInitialize() { return true; }
        virtual void implFinalize() {}
    private:
        bool setState(int32 id, B1AMHSSEM::CARRIER_STATE newState, B1AMHSSEM::CARRIER_STATE* prevState);
        bool setState(int32 id, B1AMHSSEM::TRANSFER_COMMAND_STATE newState, B1AMHSSEM::TRANSFER_COMMAND_STATE* prevState);
        bool isCarrierStateInstalled(B1AMHSSEM::CARRIER_STATE state) const;
        bool isTransferStateNotActive(B1AMHSSEM::TRANSFER_COMMAND_STATE state) const;
        bool isTransferStateActive(B1AMHSSEM::TRANSFER_COMMAND_STATE state) const;
    public:
        bool initialize();
        void finalize();
        bool addData(int32 id, std::shared_ptr<B1AMHSCarrierTransferState> data);
        void removeData(int32 id);
        void setCarrierState(int32 id, B1AMHSSEM::CARRIER_STATE newState);
        void setTransferCommandState(int32 id, B1AMHSSEM::TRANSFER_COMMAND_STATE newState, void* param = NULL);
        B1AMHSSEM::CARRIER_STATE carrierState(int32 id) const;
        B1AMHSSEM::TRANSFER_COMMAND_STATE transferState(int32 id) const;
    };
}   //  !BnD

#endif  // !_B1SEM_AMHS_CARRIER_TRANSFER_STATE_REPOSITORY_H
