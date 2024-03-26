//
// B1GEMReportRepository.cpp
//
// Library: B1GEM
// Package: B1GEM
// Module:  B1GEM
//
// Written by jmin1983@gmail.com
// Feel free to use, for any purpose.
//

#include "B1GEM.h"
#include "B1GEMReportRepository.h"

#include <B1SECS2/B1SECS2Data.h>

using namespace BnD;

B1GEMReportRepository::B1GEMReportRepository()
    : _lock()
    ,  _reports()
{
}

B1GEMReportRepository::~B1GEMReportRepository()
{
}

bool B1GEMReportRepository::isDeleteAllReportsRequested(const std::map<B1SECS2DataRPTID, std::list<std::shared_ptr<B1SECS2DataVID> > >& newReports) const
{
    return newReports.empty();
}

bool B1GEMReportRepository::isDeleteReportRequested(const std::list<std::shared_ptr<B1SECS2DataVID> >& newVIDs) const
{
    return newVIDs.empty();
}

B1GEMReportRepository::RESULT B1GEMReportRepository::updateReport(const std::map<B1SECS2DataRPTID, std::list<std::shared_ptr<B1SECS2DataVID> > >& newReports)
{
    bool deleteAllReportsRequested = isDeleteAllReportsRequested(newReports);
    B1AutoLock al(_lock);
    if (deleteAllReportsRequested) {
        _reports.clear();
    }
    else {
        std::map<B1SECS2DataRPTID, SPGEMReport> reports;
        for (const auto& reportsPair : _reports) {
            reports.insert(std::make_pair(reportsPair.first, reportsPair.second->clone()));
        }
        for (const auto& newReportsPair : newReports) {
            auto itrReports = reports.find(newReportsPair.first);
            if (itrReports != reports.end()) {
                if (isDeleteReportRequested(newReportsPair.second)) {
                    reports.erase(itrReports);
                }
                else {
                    B1LOG("updateReport failed. Already defined: [%s]", newReportsPair.first->toString().cString());
                    assert(false);
                    return RESULT_AT_LEAST_ONE_RPTID_ALREADY_DEFINED;
                }
            }
            else {
                if (newReportsPair.second.empty() != true) {
                    reports.insert(std::make_pair(newReportsPair.first, std::make_shared<B1GEMReport>(newReportsPair.first, newReportsPair.second)));
                }
                else {
                    B1LOG("updateReport failed. VID is empty: RPTID[%s]", newReportsPair.first->toString().cString());
                    //return RESULT_AT_LEAST_ONE_VID_DOES_NOT_EXIST;
                }
            }
        }
        reports.swap(_reports);
    }
    return RESULT_ACCEPTED;
}

SPGEMReport B1GEMReportRepository::findReport(const B1SECS2DataRPTID& rptID) const
{
    B1AutoLock al(_lock);
    auto itr = _reports.find(rptID);
    return itr != _reports.end() ? itr->second : SPGEMReport();
}

std::vector<SPGEMReport> B1GEMReportRepository::findReports(const std::vector<std::shared_ptr<B1SECS2DataRPTID> >& rptIDs) const
{
    B1AutoLock al(_lock);
    std::vector<SPGEMReport> reports;
    reports.reserve(rptIDs.size());
    for (auto rptID : rptIDs) {
        auto itr = _reports.find(*rptID);
        if (itr != _reports.end()) {
            reports.push_back(itr->second);
        }
    }
    return reports;
}
