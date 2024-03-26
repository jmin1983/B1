//
// B1AMHSVehicleStateRepositoryListener.h
//
// Library: B1SEM
// Package: B1SEM
// Module:  B1SEM
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1SEM_AMHS_VEHICLE_STATE_REPOSITORY_LISTENER_H
#define _B1SEM_AMHS_VEHICLE_STATE_REPOSITORY_LISTENER_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

namespace BnD {
    namespace B1AMHSSEM {
        enum class VEHICLE_ACT_STATE;
        enum class VEHICLE_AVAIL_STATE;
    };
    class B1AMHSVehicleStateRepositoryListener {
    public:
        virtual void onActionVehicleArrived(int32 id, B1AMHSSEM::VEHICLE_ACT_STATE prevState, B1AMHSSEM::VEHICLE_ACT_STATE newState) {}
        virtual void onActionVehicleDeparted(int32 id, B1AMHSSEM::VEHICLE_ACT_STATE prevState, B1AMHSSEM::VEHICLE_ACT_STATE newState) {}
        virtual void onActionVehicleAcquireStarted(int32 id, B1AMHSSEM::VEHICLE_ACT_STATE prevState, B1AMHSSEM::VEHICLE_ACT_STATE newState) {}
        virtual void onActionVehicleAcquireCompleted(int32 id, B1AMHSSEM::VEHICLE_ACT_STATE prevState, B1AMHSSEM::VEHICLE_ACT_STATE newState) {}
        virtual void onActionVehicleDepositStarted(int32 id, B1AMHSSEM::VEHICLE_ACT_STATE prevState, B1AMHSSEM::VEHICLE_ACT_STATE newState) {}
        virtual void onActionVehicleDepositCompleted(int32 id, B1AMHSSEM::VEHICLE_ACT_STATE prevState, B1AMHSSEM::VEHICLE_ACT_STATE newState) {}
        virtual void onActionVehicleUnassigned(int32 id, B1AMHSSEM::VEHICLE_ACT_STATE prevState, B1AMHSSEM::VEHICLE_ACT_STATE newState) {}
        virtual void onActionVehicleAssigned(int32 id, B1AMHSSEM::VEHICLE_ACT_STATE prevState, B1AMHSSEM::VEHICLE_ACT_STATE newState) {}
        virtual void onActionVehicleRemoved(int32 id, B1AMHSSEM::VEHICLE_ACT_STATE prevState, B1AMHSSEM::VEHICLE_ACT_STATE newState) {}
        virtual void onActionVehicleInstalled(int32 id, B1AMHSSEM::VEHICLE_ACT_STATE prevState, B1AMHSSEM::VEHICLE_ACT_STATE newState) {}
        virtual void onActionVehicleETC(int32 id, B1AMHSSEM::VEHICLE_ACT_STATE prevState, B1AMHSSEM::VEHICLE_ACT_STATE newState) { assert(false); }
        virtual void onActionVehicleInService(int32 id, B1AMHSSEM::VEHICLE_AVAIL_STATE prevState, B1AMHSSEM::VEHICLE_AVAIL_STATE newState) {}
        virtual void onActionVehicleOutOfService(int32 id, B1AMHSSEM::VEHICLE_AVAIL_STATE prevState, B1AMHSSEM::VEHICLE_AVAIL_STATE newState) {}
    };
}   //  !BnD

#endif  // !_B1SEM_AMHS_VEHICLE_STATE_REPOSITORY_LISTENER_H
