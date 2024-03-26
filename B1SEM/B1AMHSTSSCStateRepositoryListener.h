//
// B1AMHSTSSCStateRepositoryListener.h
//
// Library: B1SEM
// Package: B1SEM
// Module:  B1SEM
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1SEM_AMHS_TSSC_STATE_REPOSITORY_LISTENER_H
#define _B1SEM_AMHS_TSSC_STATE_REPOSITORY_LISTENER_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

namespace BnD {
    namespace B1AMHSSEM {
        enum class SYSTEM_STATE;
        enum class SYSTEM_AVAIL_STATE;
    };
    class B1AMHSTSSCStateRepositoryListener {
    public:
        virtual void onActionTSSCPaused(B1AMHSSEM::SYSTEM_STATE prevState, B1AMHSSEM::SYSTEM_STATE newState) {}
        virtual void onActionTSSCAutoInitiated(B1AMHSSEM::SYSTEM_STATE prevState, B1AMHSSEM::SYSTEM_STATE newState) {}
        virtual void onActionTSSCAutoCompleted(B1AMHSSEM::SYSTEM_STATE prevState, B1AMHSSEM::SYSTEM_STATE newState) {}
        virtual void onActionTSSCPauseInitiated(B1AMHSSEM::SYSTEM_STATE prevState, B1AMHSSEM::SYSTEM_STATE newState) {}
        virtual void onActionTSSCPauseCompleted(B1AMHSSEM::SYSTEM_STATE prevState, B1AMHSSEM::SYSTEM_STATE newState) {}
        virtual void onActionTSSCETC(B1AMHSSEM::SYSTEM_STATE prevState, B1AMHSSEM::SYSTEM_STATE newState) { assert(false); }
        virtual void onActionTSSCInService(B1AMHSSEM::SYSTEM_AVAIL_STATE prevState, B1AMHSSEM::SYSTEM_AVAIL_STATE newState) {}
        virtual void onActionTSSCOutOfService(B1AMHSSEM::SYSTEM_AVAIL_STATE prevState, B1AMHSSEM::SYSTEM_AVAIL_STATE newState) {}
    };
}   //  !BnD

#endif  // !_B1SEM_AMHS_TSSC_STATE_REPOSITORY_LISTENER_H
