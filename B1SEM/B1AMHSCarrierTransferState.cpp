//
// B1AMHSCarrierTransferState.cpp
//
// Library: B1SEM
// Package: B1SEM
// Module:  B1SEM
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1SEM.h"
#include "B1AMHSCarrierTransferState.h"

using namespace BnD;
using namespace B1AMHSSEM;

B1AMHSCarrierTransferState::B1AMHSCarrierTransferState(CARRIER_STATE carrierState, TRANSFER_COMMAND_STATE transferCommandState)
    : _carrierState(carrierState)
    , _transferCommandState(transferCommandState)
{
}

B1AMHSCarrierTransferState::~B1AMHSCarrierTransferState()
{
}
