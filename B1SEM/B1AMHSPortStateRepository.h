//
// B1AMHSPortStateRepository.h
//
// Library: B1SEM
// Package: B1SEM
// Module:  B1SEM
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1SEM_AMHS_PORT_STATE_REPOSITORY_H
#define _B1SEM_AMHS_PORT_STATE_REPOSITORY_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <map>

namespace BnD {
    namespace B1AMHSSEM {
        enum class PORT_STATE;
    };
    class B1AMHSPortStateRepositoryListener;
    class B1Lock;
    class B1AMHSPortStateRepository {
    public:
        B1AMHSPortStateRepository();
        virtual ~B1AMHSPortStateRepository();
    protected:
        B1AMHSPortStateRepositoryListener* _listener;
        std::shared_ptr<B1Lock> _lock;
        std::map<int32, B1AMHSSEM::PORT_STATE> _states;   //  map<port_id, states>
    protected:
        bool setState(int32 id, B1AMHSSEM::PORT_STATE newState, B1AMHSSEM::PORT_STATE* prevState);
        void setPortState(int32 id, B1AMHSSEM::PORT_STATE newState);
    public:
        bool initialize(std::map<int32, B1AMHSSEM::PORT_STATE>&& states, B1AMHSPortStateRepositoryListener* listener);
        void finalize();
        void setPortStateInService(int32 id);
        void setPortStateOutOfService(int32 id);
        bool isPortStateInService(int32 id) const;
        B1AMHSSEM::PORT_STATE portState(int32 id) const;
    };
}   //  !BnD

#endif  // !_B1SEM_AMHS_PORT_STATE_REPOSITORY_H
