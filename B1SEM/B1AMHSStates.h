//
// B1AMHSStates.h
//
// Library: B1SEM
// Package: B1SEM
// Module:  B1SEM
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1SEM_AMHS_STATES_H
#define _B1SEM_AMHS_STATES_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

namespace BnD {
    namespace B1AMHSSEM {
        enum class CARRIER_STATE {
            NONE = -1,
            INSTALLED_WAIT_IN = 0,      //  carrier is ready to be transfered to storage or output.
            INSTALLED_WAIT_OUT,         //  carrier is ready to be removed by manual or other system.
            INSTALLED_TRANSFERRING,
            INSTALLED_STORED_COMPLETED,
            INSTALLED_STORED_ALTERNATE, //  carrier is temporarily stored at a storage location. completion is pending until the destination become available.
        };

        enum class PORT_STATE {
            NONE = -1,
            OUT_OF_SERVICE = 0,
            IN_SERVICE,
        };

        enum class STORAGE_LOCATION_STATE {
            NONE = -1,
            ENABLED = 0,
            DISABLED,   //  if the Host issues a TRANSFER command which would cause the location to be accessed, the TRANSFER shall be rejected(with HCACK cannot perform now).
        };

        enum class TRANSFER_COMMAND_STATE {
            NONE = -1,
            NOT_ACTIVE_QUEUED = 0,  //  TRANSFER command has not been initiated.
            NOT_ACTIVE_WAITING,     //  TRANSFER command has been initiated. A vehicle is on its way to the source location to acquire the carrier. skip if no vehicle-based.
            ACTIVE_TRANSFERRING,
            ACTIVE_PAUSED,
            CANCELING,              //  terminate TRANSFER command which never entered the ACTIVE state(only accepted when previous state is NOT_ACTIVE).
            ABORTING,               //  terminate TRANSFER command which has entered the ACTIVE state(only accepted when previous state is ACTIVE).
        };

        enum class SYSTEM_STATE {
            NONE = -1,
            TSSC_INIT = 0,
            PAUSING,
            PAUSED,
            AUTO,
        };
        enum class SYSTEM_AVAIL_STATE {
            NONE = -1,
            IN_SERVICE = 0,
            OUT_OF_SERVICE,
        };

        enum class VEHICLE_ACT_STATE {  //  activity state
            NONE = -1,
            REMOVED,                        //  REMOVED. vehicle is not available for TRANSFER command. no events or alarms will be sent to the Host for vehicles in this state.
            INSTALLED_NOT_ASSIGNED,         //  INSTALLED. NOT ASSIGNED. vehicle is not allocated to a TRANSFER command. vehicle may contain a carrier as the result of a command being aborted.
            INSTALLED_ASSIGNED_ENROUTE,     //  INSTALLED. ASSIGNED. vehicle is on its way to a transfer location. default entry into the ASSIGNED state.
            INSTALLED_ASSIGNED_PARKED,      //  INSTALLED. ASSIGNED. after the arrival of the vehicle is completed.
            INSTALLED_ASSIGNED_ACQUIRING,   //  INSTALLED. ASSIGNED. vehicle is currently involved in carrier acquire.
            INSTALLED_ASSIGNED_DEPOSITING,  //  INSTALLED. ASSIGNED. vehicle is currently involved in carrier deposit.
        };
        enum class VEHICLE_AVAIL_STATE {
            NONE = -1,
            IN_SERVICE = 0,
            OUT_OF_SERVICE, //  state of the vehicle is not a valid reason to reject a command from the Host(maybe other reason if want to reject command).
        };
    };
}   //  !BnD

#endif  // !_B1SEM_AMHS_STATES_H
