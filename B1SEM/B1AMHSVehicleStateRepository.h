//
// B1AMHSVehicleStateRepository.h
//
// Library: B1SEM
// Package: B1SEM
// Module:  B1SEM
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1SEM_AMHS_VEHICLE_STATE_REPOSITORY_H
#define _B1SEM_AMHS_VEHICLE_STATE_REPOSITORY_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <map>

namespace BnD {
    namespace B1AMHSSEM {
        enum class VEHICLE_ACT_STATE;
        enum class VEHICLE_AVAIL_STATE;
    };
    class B1AMHSVehicleStateRepositoryListener;
    class B1Lock;
    class B1AMHSVehicleStateRepository {
    public:
        B1AMHSVehicleStateRepository();
        virtual ~B1AMHSVehicleStateRepository();
    protected:
        struct States {
            States(B1AMHSSEM::VEHICLE_ACT_STATE activityState, B1AMHSSEM::VEHICLE_AVAIL_STATE availState)
                : _activityState(activityState), _availState(availState) {}
            States(const std::pair<B1AMHSSEM::VEHICLE_ACT_STATE, B1AMHSSEM::VEHICLE_AVAIL_STATE>& state)
                : _activityState(state.first), _availState(state.second) {}
            B1AMHSSEM::VEHICLE_ACT_STATE _activityState;
            B1AMHSSEM::VEHICLE_AVAIL_STATE _availState;
        };
    protected:
        B1AMHSVehicleStateRepositoryListener* _listener;
        std::shared_ptr<B1Lock> _lock;
        std::map<int32, States> _states;    //  map<vehicle_id, states>
    protected:
        bool setState(int32 id, B1AMHSSEM::VEHICLE_ACT_STATE newState, B1AMHSSEM::VEHICLE_ACT_STATE* prevState);
        bool setState(int32 id, B1AMHSSEM::VEHICLE_AVAIL_STATE newState, B1AMHSSEM::VEHICLE_AVAIL_STATE* prevState);
        bool isStateAssigned(B1AMHSSEM::VEHICLE_ACT_STATE state) const;
        bool isStateInstalled(B1AMHSSEM::VEHICLE_ACT_STATE state) const;
    public:
        bool initialize(const std::map<int32, std::pair<B1AMHSSEM::VEHICLE_ACT_STATE, B1AMHSSEM::VEHICLE_AVAIL_STATE> >& states,
                        B1AMHSVehicleStateRepositoryListener* listener);
        void finalize();
        void setVehicleActivityState(int32 id, B1AMHSSEM::VEHICLE_ACT_STATE newState);
        void setVehicleAvailState(int32 id, B1AMHSSEM::VEHICLE_AVAIL_STATE newState);
        B1AMHSSEM::VEHICLE_ACT_STATE vehicleActivityState(int32 id) const;
        B1AMHSSEM::VEHICLE_AVAIL_STATE vehicleAvailState(int32 id) const;
    };
}   //  !BnD

#endif  // !_B1SEM_AMHS_VEHICLE_STATE_REPOSITORY_H
