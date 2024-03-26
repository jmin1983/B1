//
// B1GEMDataValueCondition.h
//
// Library: B1GEM
// Package: B1GEM
// Module:  B1GEM
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1GEM_VALUE_CONDITION_H
#define _B1GEM_VALUE_CONDITION_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

namespace BnD {
    struct B1GEMDataValueCondition {
        B1GEMDataValueCondition() : _taskID(-1), _locationID(-1), _hostID(-1) {}
        B1GEMDataValueCondition(int32 taskID, int32 locationID, int32 hostID = -1) : _taskID(taskID), _locationID(locationID), _hostID(hostID) {}
        virtual ~B1GEMDataValueCondition() {}
        const int32 _taskID;
        const int32 _locationID;
        const int32 _hostID;
        int32 taskID() const { return _taskID; }
        int32 locationID() const { return _locationID; }
        int32 hostID() const { return _hostID; }
        bool hasValidTaskID() const { return _taskID > -1; }
        bool hasValidLocationID() const { return _locationID > -1; }
        bool hasValidHostID() const { return _hostID > -1; }
        B1String toString() const { return B1String::formatAs("[%d][%d][%d]", taskID(), locationID(), hostID()); }
    };
    struct B1GEMDataValueConditionTaskID : B1GEMDataValueCondition {
        B1GEMDataValueConditionTaskID(int32 taskID) : B1GEMDataValueCondition(taskID, -1, -1) {}
    };
    struct B1GEMDataValueConditionLocationID : B1GEMDataValueCondition {
        B1GEMDataValueConditionLocationID(int32 locationID) : B1GEMDataValueCondition(-1, locationID, -1) {}
    };
    struct B1GEMDataValueConditionHostID : B1GEMDataValueCondition {
        B1GEMDataValueConditionHostID(int32 hostID) : B1GEMDataValueCondition(-1, -1, hostID) {}
    };
}   //  !BnD

#endif  // !_B1GEM_VALUE_CONDITION_H
