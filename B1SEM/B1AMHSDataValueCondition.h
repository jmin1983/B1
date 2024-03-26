//
// B1AMHSDataValueCondition.h
//
// Library: B1SEM
// Package: B1SEM
// Module:  B1SEM
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1SEM_AMHS_DATA_VALUE_CONDITION_H
#define _B1SEM_AMHS_DATA_VALUE_CONDITION_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <B1GEM/B1GEMDataValueCondition.h>

namespace BnD {
    class B1AMHSCarrierTransferStateRepository;
    class B1AMHSPortStateRepository;
    class B1AMHSStorageLocationStateRepository;
    class B1AMHSTSSCStateRepository;
    class B1AMHSVehicleStateRepository;
    struct B1AMHSDataValueCondition : B1GEMDataValueCondition {
        B1AMHSDataValueCondition(B1AMHSCarrierTransferStateRepository* carrierTransferStateRepository,
                                 B1AMHSPortStateRepository* portStateRepository,
                                 B1AMHSStorageLocationStateRepository* storageLocationStateRepository,
                                 B1AMHSTSSCStateRepository* systemStateRepository,
                                 B1AMHSVehicleStateRepository* vehicleStateRepository)
            : B1GEMDataValueCondition()
            , _carrierTransferStateRepository(carrierTransferStateRepository)
            , _portStateRepository(portStateRepository)
            , _storageLocationStateRepository(storageLocationStateRepository)
            , _systemStateRepository(systemStateRepository)
            , _vehicleStateRepository(vehicleStateRepository) {}
        B1AMHSDataValueCondition(B1AMHSCarrierTransferStateRepository* carrierTransferStateRepository,
                                 B1AMHSPortStateRepository* portStateRepository,
                                 B1AMHSStorageLocationStateRepository* storageLocationStateRepository,
                                 B1AMHSTSSCStateRepository* systemStateRepository,
                                 B1AMHSVehicleStateRepository* vehicleStateRepository,
                                 int32 taskID, int32 locationID, int32 hostID = -1)
            : B1GEMDataValueCondition(taskID, locationID, hostID)
            , _carrierTransferStateRepository(carrierTransferStateRepository)
            , _portStateRepository(portStateRepository)
            , _storageLocationStateRepository(storageLocationStateRepository)
            , _systemStateRepository(systemStateRepository)
            , _vehicleStateRepository(vehicleStateRepository) {}
        B1AMHSDataValueCondition(const B1GEMDataValueCondition& gemCondition,
                                 B1AMHSCarrierTransferStateRepository* carrierTransferStateRepository,
                                 B1AMHSPortStateRepository* portStateRepository,
                                 B1AMHSStorageLocationStateRepository* storageLocationStateRepository,
                                 B1AMHSTSSCStateRepository* systemStateRepository,
                                 B1AMHSVehicleStateRepository* vehicleStateRepository)
            : B1GEMDataValueCondition(gemCondition)
            , _carrierTransferStateRepository(carrierTransferStateRepository)
            , _portStateRepository(portStateRepository)
            , _storageLocationStateRepository(storageLocationStateRepository)
            , _systemStateRepository(systemStateRepository)
            , _vehicleStateRepository(vehicleStateRepository) {}
        B1AMHSCarrierTransferStateRepository* _carrierTransferStateRepository;
        B1AMHSPortStateRepository* _portStateRepository;
        B1AMHSStorageLocationStateRepository* _storageLocationStateRepository;
        B1AMHSTSSCStateRepository* _systemStateRepository;
        B1AMHSVehicleStateRepository* _vehicleStateRepository;
    };
    struct B1AMHSDataValueConditionTaskID : B1AMHSDataValueCondition {
        B1AMHSDataValueConditionTaskID(B1AMHSCarrierTransferStateRepository* carrierTransferStateRepository,
                                       B1AMHSPortStateRepository* portStateRepository,
                                       B1AMHSStorageLocationStateRepository* storageLocationStateRepository,
                                       B1AMHSTSSCStateRepository* systemStateRepository,
                                       B1AMHSVehicleStateRepository* vehicleStateRepository,
                                       int32 taskID)
            : B1AMHSDataValueCondition(carrierTransferStateRepository,
                                       portStateRepository,
                                       storageLocationStateRepository,
                                       systemStateRepository,
                                       vehicleStateRepository,
                                       taskID, -1, -1) {}
    };
    struct B1AMHSDataValueConditionLocationID : B1AMHSDataValueCondition {
        B1AMHSDataValueConditionLocationID(B1AMHSCarrierTransferStateRepository* carrierTransferStateRepository,
                                           B1AMHSPortStateRepository* portStateRepository,
                                           B1AMHSStorageLocationStateRepository* storageLocationStateRepository,
                                           B1AMHSTSSCStateRepository* systemStateRepository,
                                           B1AMHSVehicleStateRepository* vehicleStateRepository,
                                           int32 locationID)
            : B1AMHSDataValueCondition(carrierTransferStateRepository,
                                       portStateRepository,
                                       storageLocationStateRepository,
                                       systemStateRepository,
                                       vehicleStateRepository,
                                       -1, locationID, -1) {}
    };
    struct B1AMHSDataValueConditionHostID : B1AMHSDataValueCondition {
        B1AMHSDataValueConditionHostID(B1AMHSCarrierTransferStateRepository* carrierTransferStateRepository,
                                       B1AMHSPortStateRepository* portStateRepository,
                                       B1AMHSStorageLocationStateRepository* storageLocationStateRepository,
                                       B1AMHSTSSCStateRepository* systemStateRepository,
                                       B1AMHSVehicleStateRepository* vehicleStateRepository,
                                       int32 hostID)
            : B1AMHSDataValueCondition(carrierTransferStateRepository,
                                       portStateRepository,
                                       storageLocationStateRepository,
                                       systemStateRepository,
                                       vehicleStateRepository,
                                       -1, -1, hostID) {}
    };
}   //  !BnD

#endif  // !_B1SEM_AMHS_DATA_VALUE_CONDITION_H
