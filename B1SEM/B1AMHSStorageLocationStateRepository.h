//
// B1AMHSStorageLocationStateRepository.h
//
// Library: B1SEM
// Package: B1SEM
// Module:  B1SEM
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1SEM_AMHS_STORAGE_LOCATION_STATE_REPOSITORY_H
#define _B1SEM_AMHS_STORAGE_LOCATION_STATE_REPOSITORY_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <map>

namespace BnD {
    namespace B1AMHSSEM {
        enum class STORAGE_LOCATION_STATE;
    };
    class B1AMHSStorageLocationStateRepositoryListener;
    class B1Lock;
    class B1AMHSStorageLocationStateRepository {
    public:
        B1AMHSStorageLocationStateRepository();
        virtual ~B1AMHSStorageLocationStateRepository();
    protected:
        B1AMHSStorageLocationStateRepositoryListener* _listener;
        std::shared_ptr<B1Lock> _lock;
        std::map<int32, B1AMHSSEM::STORAGE_LOCATION_STATE> _states;   //  map<location_id, states>
    protected:
        bool setState(int32 id, B1AMHSSEM::STORAGE_LOCATION_STATE newState, B1AMHSSEM::STORAGE_LOCATION_STATE* prevState);
        void setStorageLocationState(int32 id, B1AMHSSEM::STORAGE_LOCATION_STATE newState);
    public:
        bool initialize(const std::map<int32, B1AMHSSEM::STORAGE_LOCATION_STATE>& states, B1AMHSStorageLocationStateRepositoryListener* listener);
        void finalize();
        void setStorageLocationStateEnabled(int32 id);
        void setStorageLocationStateDisabled(int32 id);
        bool isStorageLocationStateEnabled(int32 id) const;
        B1AMHSSEM::STORAGE_LOCATION_STATE storageLocationState(int32 id) const;
    };
}   //  !BnD

#endif  // !_B1SEM_AMHS_STORAGE_LOCATION_STATE_REPOSITORY_H
