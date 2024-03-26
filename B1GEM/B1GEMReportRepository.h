//
// B1GEMReportRepository.h
//
// Library: B1GEM
// Package: B1GEM
// Module:  B1GEM
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1GEM_REPORT_REPOSITORY_H
#define _B1GEM_REPORT_REPOSITORY_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <B1Base/B1Lock.h>
#include <B1GEM/B1GEMReport.h>

#include <map>

namespace BnD {
    class B1GEMReportRepository {
    public:
        B1GEMReportRepository();
        virtual ~B1GEMReportRepository();
    public:
        enum RESULT {
            RESULT_ACCEPTED = 0,
            RESULT_INSUFFCIENT_SPACE = 1,
            RESULT_INVALID_FORMAT = 2,
            RESULT_AT_LEAST_ONE_RPTID_ALREADY_DEFINED = 3,
            RESULT_AT_LEAST_ONE_VID_DOES_NOT_EXIST = 4,
        };
    protected:
        mutable B1Lock _lock;
        std::map<B1SECS2DataRPTID, SPGEMReport> _reports;
    protected:
        bool isDeleteAllReportsRequested(const std::map<B1SECS2DataRPTID, std::list<std::shared_ptr<B1SECS2DataVID> > >& newReports) const;
        bool isDeleteReportRequested(const std::list<std::shared_ptr<B1SECS2DataVID> >& newVIDs) const;
    public:
        RESULT updateReport(const std::map<B1SECS2DataRPTID, std::list<std::shared_ptr<B1SECS2DataVID> > >& newReports);
        SPGEMReport findReport(const B1SECS2DataRPTID& rptID) const;
        std::vector<SPGEMReport> findReports(const std::vector<std::shared_ptr<B1SECS2DataRPTID> >& rptIDs) const;
    };
}   //  !BnD

#endif  // !_B1GEM_REPORT_REPOSITORY_H
