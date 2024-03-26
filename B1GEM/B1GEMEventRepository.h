//
// B1GEMEventRepository.h
//
// Library: B1GEM
// Package: B1GEM
// Module:  B1GEM
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1GEM_EVENT_REPOSITORY_H
#define _B1GEM_EVENT_REPOSITORY_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <B1Base/B1Lock.h>
#include <B1GEM/B1GEMEvent.h>

#include <map>
#include <set>

namespace BnD {
    class B1GEMEventRepository {
    public:
        B1GEMEventRepository();
        virtual ~B1GEMEventRepository();
    public:
        enum RESULT {
            RESULT_ACCEPTED = 0,
            RESULT_AT_LEAST_ONE_CEID_DOES_NOT_EXIST = 1,
            RESULT_BUSY = 2,
            RESULT_INSUFFICIENT_SPACE = 3,
            RESULT_INVALID_FORMAT = 4,
            RESULT_AT_LEAST_ONE_CEID_LINK_IS_ALREADY_DEFINED = 5,
            RESULT_AT_LEAST_ONE_RPTID_DOES_NOT_EXIST = 6,
        };
    protected:
        mutable B1Lock _lock;
        std::map<const B1SECS2DataCEID, SPGEMEvent> _events;
    protected:
        bool isGetAllRequested(const std::vector<B1SECS2DataCEID>& ceIDs) const;
    public:
        bool initialize(const std::set<B1GEMEvent>& events);
        void finalize();
        void deleteAllReports();
        RESULT linkReport(const std::map<B1SECS2DataCEID, std::list<std::shared_ptr<B1SECS2DataRPTID> > >& linkData);
        bool getAvailableCEIDs(const std::vector<B1SECS2DataCEID>& ceIDs, std::vector<B1SECS2DataCEID>* result) const;
        SPGEMEvent findEvent(const B1SECS2DataCEID& ceID) const;
    };
}   //  !BnD

#endif  // !_B1GEM_EVENT_REPOSITORY_H
