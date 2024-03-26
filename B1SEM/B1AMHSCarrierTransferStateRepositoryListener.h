//
// B1AMHSCarrierTransferStateRepositoryListener.h
//
// Library: B1SEM
// Package: B1SEM
// Module:  B1SEM
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1SEM_AMHS_CARRIER_TRANSFER_STATE_REPOSITORY_LISTENER_H
#define _B1SEM_AMHS_CARRIER_TRANSFER_STATE_REPOSITORY_LISTENER_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

namespace BnD {
    namespace B1AMHSSEM {
        enum class CARRIER_STATE;
        enum class TRANSFER_COMMAND_STATE;
    };
    class B1AMHSCarrierTransferStateRepositoryListener {
    public:
        virtual void onActionCarrierWaitIn(int32 id, B1AMHSSEM::CARRIER_STATE prevState, B1AMHSSEM::CARRIER_STATE newState) {}
        virtual void onActionCarrierTransferring(int32 id, B1AMHSSEM::CARRIER_STATE prevState, B1AMHSSEM::CARRIER_STATE newState) {}
        virtual void onActionCarrierStoredAlt(int32 id, B1AMHSSEM::CARRIER_STATE prevState, B1AMHSSEM::CARRIER_STATE newState) {}
        virtual void onActionCarrierStored(int32 id, B1AMHSSEM::CARRIER_STATE prevState, B1AMHSSEM::CARRIER_STATE newState) {}
        virtual void onActionCarrierResumed(int32 id, B1AMHSSEM::CARRIER_STATE prevState, B1AMHSSEM::CARRIER_STATE newState) {}
        virtual void onActionCarrierWaitOut(int32 id, B1AMHSSEM::CARRIER_STATE prevState, B1AMHSSEM::CARRIER_STATE newState) {}
        virtual void onActionCarrierRemoved(int32 id, B1AMHSSEM::CARRIER_STATE prevState, B1AMHSSEM::CARRIER_STATE newState) {}
        virtual void onActionCarrierInstallCompleted(int32 id, B1AMHSSEM::CARRIER_STATE prevState, B1AMHSSEM::CARRIER_STATE newState) {}
        virtual void onActionCarrierRemoveCompleted(int32 id, B1AMHSSEM::CARRIER_STATE prevState, B1AMHSSEM::CARRIER_STATE newState) {}
        virtual void onActionCarrierETC(int32 id, B1AMHSSEM::CARRIER_STATE prevState, B1AMHSSEM::CARRIER_STATE newState) { assert(false); }

        virtual void onActionTransferInitiated(int32 id, B1AMHSSEM::TRANSFER_COMMAND_STATE prevState, B1AMHSSEM::TRANSFER_COMMAND_STATE newState) {}
        virtual void onActionTransferring(int32 id, B1AMHSSEM::TRANSFER_COMMAND_STATE prevState, B1AMHSSEM::TRANSFER_COMMAND_STATE newState) {}
        virtual void onActionTransferCancelInitiated(int32 id, B1AMHSSEM::TRANSFER_COMMAND_STATE prevState, B1AMHSSEM::TRANSFER_COMMAND_STATE newState) {}
        virtual void onActionTransferCancelFailed(int32 id, B1AMHSSEM::TRANSFER_COMMAND_STATE prevState, B1AMHSSEM::TRANSFER_COMMAND_STATE newState) {}
        virtual void onActionTransferCancelCompleted(int32 id, B1AMHSSEM::TRANSFER_COMMAND_STATE prevState, B1AMHSSEM::TRANSFER_COMMAND_STATE newState) {}
        virtual void onActionTransferPaused(int32 id, B1AMHSSEM::TRANSFER_COMMAND_STATE prevState, B1AMHSSEM::TRANSFER_COMMAND_STATE newState) {}
        virtual void onActionTransferResumed(int32 id, B1AMHSSEM::TRANSFER_COMMAND_STATE prevState, B1AMHSSEM::TRANSFER_COMMAND_STATE newState) {}
        virtual void onActionTransferAbortInitiated(int32 id, B1AMHSSEM::TRANSFER_COMMAND_STATE prevState, B1AMHSSEM::TRANSFER_COMMAND_STATE newState) {}
        virtual void onActionTransferAbortFailed(int32 id, B1AMHSSEM::TRANSFER_COMMAND_STATE prevState, B1AMHSSEM::TRANSFER_COMMAND_STATE newState) {}
        virtual void onActionTransferAbortCompleted(int32 id, B1AMHSSEM::TRANSFER_COMMAND_STATE prevState, B1AMHSSEM::TRANSFER_COMMAND_STATE newState) {}
        virtual void onActionTransferCompleted(int32 id, B1AMHSSEM::TRANSFER_COMMAND_STATE prevState, B1AMHSSEM::TRANSFER_COMMAND_STATE newState, void* param) {}
        virtual void onActionTransferETC(int32 id, B1AMHSSEM::TRANSFER_COMMAND_STATE prevState, B1AMHSSEM::TRANSFER_COMMAND_STATE newState) { assert(false); }
    };
}   //  !BnD

#endif  // !_B1SEM_AMHS_CARRIER_TRANSFER_STATE_REPOSITORY_LISTENER_H
