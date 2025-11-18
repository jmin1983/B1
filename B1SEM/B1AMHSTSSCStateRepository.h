//
// B1AMHSTSSCStateRepository.h
//
// Library: B1SEM
// Package: B1SEM
// Module:  B1SEM
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1SEM_AMHS_TSSC_STATE_REPOSITORY_H
#define _B1SEM_AMHS_TSSC_STATE_REPOSITORY_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

namespace BnD {
    namespace B1AMHSSEM {
        enum class SYSTEM_STATE;
        enum class SYSTEM_AVAIL_STATE;
    };
    class B1AMHSTSSCStateRepositoryListener;
    class B1Lock;
    class B1AMHSTSSCStateRepository {
    public:
        B1AMHSTSSCStateRepository();
        virtual ~B1AMHSTSSCStateRepository();
    protected:
        B1AMHSTSSCStateRepositoryListener* _listener;
        std::shared_ptr<B1Lock> _lock;
        B1AMHSSEM::SYSTEM_STATE _systemState;
        B1AMHSSEM::SYSTEM_AVAIL_STATE _systemAvailState;
    protected:
        bool setSystemState(B1AMHSSEM::SYSTEM_STATE newState);
        void setSystemAvailState(B1AMHSSEM::SYSTEM_AVAIL_STATE newState);
    public:
        bool initialize(B1AMHSTSSCStateRepositoryListener* listener);
        void finalize();
        bool setSystemStateInit();
        bool setSystemStatePausing();
        bool setSystemStatePaused();
        bool setSystemStateAuto();
        void setSystemAvailStateInService();
        void setSystemAvailStateOutOfService();
        bool isSystemState(B1AMHSSEM::SYSTEM_STATE state) const;
        bool isSystemStatePausing() const;
        bool isSystemStatePaused() const;
        bool isSystemStateAuto() const;
        bool isSystemAvailState(B1AMHSSEM::SYSTEM_AVAIL_STATE state) const;
        bool isSystemAvailStateInService() const;
        bool isSystemAvailStateOutOfService() const;
        B1AMHSSEM::SYSTEM_STATE systemState() const;
        B1AMHSSEM::SYSTEM_AVAIL_STATE systemAvailState() const;
    };
}   //  !BnD

#endif  // !_B1SEM_AMHS_TSSC_STATE_REPOSITORY_H
