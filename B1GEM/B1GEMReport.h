//
// B1GEMReport.h
//
// Library: B1GEM
// Package: B1GEM
// Module:  B1GEM
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#ifndef _B1GEM_REPORT_H
#define _B1GEM_REPORT_H

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <B1SECS2/B1SECS2DataTypes.h>

namespace BnD {
    class B1GEMReport {
    protected:
        B1GEMReport(const B1GEMReport& report, bool deepCopy);
    public:
        B1GEMReport(const B1SECS2DataRPTID& rptID, const std::list<std::shared_ptr<B1SECS2DataVID> >& vIDs);
        virtual ~B1GEMReport();
    protected:
        std::shared_ptr<const B1SECS2DataRPTID> _rptID;
        std::list<std::shared_ptr<B1SECS2DataVID> > _vIDs;
    public:
        bool operator<(const B1GEMReport& r) const { return *_rptID < *r._rptID; }
        B1GEMReport* clone() { return new B1GEMReport(*this, true); }
    public:
        const B1SECS2DataRPTID& rptID() const { return *_rptID; }
        const std::list<std::shared_ptr<B1SECS2DataVID> >& vIDs() const { return _vIDs; }
    };

    typedef std::shared_ptr<B1GEMReport> SPGEMReport;
}   //  !BnD

#endif  // !_B1GEM_REPORT_H
