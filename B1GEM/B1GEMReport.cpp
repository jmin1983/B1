//
// B1GEMReport.cpp
//
// Library: B1GEM
// Package: B1GEM
// Module:  B1GEM
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1GEM.h"
#include "B1GEMReport.h"

using namespace BnD;

B1GEMReport::B1GEMReport(const B1GEMReport& report, bool deepCopy)
    : _rptID(static_cast<const B1SECS2DataRPTID*>(report._rptID->clone()))
    , _vIDs()
{
    for (auto vID : report._vIDs) {
        _vIDs.push_back(std::shared_ptr<B1SECS2DataVID>(static_cast<B1SECS2DataVID*>(vID->clone())));
    }
}


B1GEMReport::B1GEMReport(const B1SECS2DataRPTID& rptID, const std::list<std::shared_ptr<B1SECS2DataVID> >& vIDs)
    : _rptID(std::make_shared<const B1SECS2DataRPTID>(rptID))
    , _vIDs(vIDs)
{
}

B1GEMReport::~B1GEMReport()
{
}
