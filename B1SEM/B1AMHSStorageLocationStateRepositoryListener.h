//
// B1AMHSStorageLocationStateRepositoryListener.h
//
// Library: B1SEM
// Package: B1SEM
// Module:  B1SEM
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1SEM_AMHS_STORAGE_LOCATION_STATE_REPOSITORY_LISTENER_H
#define _B1SEM_AMHS_STORAGE_LOCATION_STATE_REPOSITORY_LISTENER_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

namespace BnD {
    namespace B1AMHSSEM {
        enum class STORAGE_LOCATION_STATE;
    };
    class B1AMHSStorageLocationStateRepositoryListener {
    public:
        virtual void onActionStorageLocationStatusChanged(int32 id, B1AMHSSEM::STORAGE_LOCATION_STATE prevState, B1AMHSSEM::STORAGE_LOCATION_STATE newState) {}
    };
}   //  !BnD

#endif  // !_B1SEM_AMHS_STORAGE_LOCATION_STATE_REPOSITORY_LISTENER_H
