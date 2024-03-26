//
// B1AMHSPortStateRepositoryListener.h
//
// Library: B1SEM
// Package: B1SEM
// Module:  B1SEM
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1SEM_AMHS_PORT_STATE_REPOSITORY_LISTENER_H
#define _B1SEM_AMHS_PORT_STATE_REPOSITORY_LISTENER_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

namespace BnD {
    namespace B1AMHSSEM {
        enum class PORT_STATE;
    };
    class B1AMHSPortStateRepositoryListener {
    public:
        virtual void onActionPortInService(int32 id, B1AMHSSEM::PORT_STATE prevState, B1AMHSSEM::PORT_STATE newState) {}
        virtual void onActionPortOutOfService(int32 id, B1AMHSSEM::PORT_STATE prevState, B1AMHSSEM::PORT_STATE newState) {}
    };
}   //  !BnD

#endif  // !_B1SEM_AMHS_PORT_STATE_REPOSITORY_LISTENER_H
