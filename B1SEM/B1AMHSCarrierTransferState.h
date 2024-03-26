//
// B1AMHSCarrierTransferState.h
//
// Library: B1SEM
// Package: B1SEM
// Module:  B1SEM
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1SEM_AMHS_CARRIER_TRANSFER_STATE_H
#define _B1SEM_AMHS_CARRIER_TRANSFER_STATE_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

namespace BnD {
    namespace B1AMHSSEM {
        enum class CARRIER_STATE;
        enum class TRANSFER_COMMAND_STATE;
    };
    class B1AMHSCarrierTransferState {
    public:
        B1AMHSCarrierTransferState(B1AMHSSEM::CARRIER_STATE carrierState, B1AMHSSEM::TRANSFER_COMMAND_STATE transferCommandState);
        virtual ~B1AMHSCarrierTransferState();
    protected:
        B1AMHSSEM::CARRIER_STATE _carrierState;
        B1AMHSSEM::TRANSFER_COMMAND_STATE _transferCommandState;
    public:
        void setCarrierState(B1AMHSSEM::CARRIER_STATE newState) { _carrierState = newState; }
        void setTransferCommandState(B1AMHSSEM::TRANSFER_COMMAND_STATE newState) { _transferCommandState = newState; }
        bool isCarrierState(B1AMHSSEM::CARRIER_STATE state) const { return _carrierState == state; }
        bool isTransferCommandState(B1AMHSSEM::TRANSFER_COMMAND_STATE state) const { return _transferCommandState == state; }
        B1AMHSSEM::CARRIER_STATE carrierState() const { return _carrierState; }
        B1AMHSSEM::TRANSFER_COMMAND_STATE transferCommandState() const { return _transferCommandState; }
    };
}   //  !BnD

#endif  // !_B1SEM_AMHS_CARRIER_TRANSFER_STATE_H
